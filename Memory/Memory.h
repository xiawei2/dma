#pragma once
#include "../pch.h"
#include "InputManager.h"
#include "Registry.h"
#include "Shellcode.h"
#include "../nt/structs.h"

class Memory
{
private:
	struct LibModules
	{
		HMODULE VMM = nullptr;
		HMODULE FTD3XX = nullptr;
		HMODULE LEECHCORE = nullptr;
	};

	static inline LibModules modules {nullptr,nullptr,nullptr };

	struct CurrentProcessInformation
	{
		int PID = 0;
		size_t base_address = 0;
		size_t base_size = 0;
		std::string process_name = "";
	};

	static inline CurrentProcessInformation current_process {0,0,0,"" };

	static inline BOOLEAN DMA_INITIALIZED = FALSE;
	static inline BOOLEAN PROCESS_INITIALIZED = FALSE;
	/**
	*Dumps the systems Current physical memory pages
	*To a file so we can use it in our DMA (:
	*This file it created to %temp% folder
	*@return true if successful, false if not.
	*/
	bool DumpMemoryMap(bool debug = false);

	/**
	* brief Removes basic information related to the FPGA device
	* This is required before any DMA operations can be done.
	* To ensure the optimal safety in game cheating.
	* @return true if successful, false if not.
	*/
	bool SetDevice();

	//shared pointer
	std::shared_ptr<c_keys> key;
	c_registry registry;
	c_shellcode shellcode;

	/*this->registry_ptr = std::make_shared<c_registry>(*this);
	this->key_ptr = std::make_shared<c_keys>(*this);*/

public:
	/**
	 * brief Constructor takes a wide string of the process.
	 * Expects that all the libraries are in the root dir
	 */
	Memory();
    Memory(std::string process_name, bool memMap = true, bool debug = false);
	~Memory();

	/**
	* @brief Gets the registry object
	* @return registry class
	*/
	c_registry GetRegistry() { return registry; }

	/**
	* @brief Gets the key object
	* @return key class
	*/
	c_keys* GetKeyboard() { return key.get(); }

	/**
	* @brief Gets the shellcode object
	* @return shellcode class
	*/
	c_shellcode GetShellcode() { return shellcode; }

	/**
	* brief Initializes the DMA
	* This is required before any DMA operations can be done.
	* @param process_name the name of the process
	* @param memMap if true, will dump the memory map to a file	& make the DMA use it.
	* @return true if successful, false if not.
	*/
	bool Init(std::string process_name, bool memMap = true, bool debug = false);

	/*This part here is things related to the process information such as Base daddy, Size ect.*/

	/**
	* brief Gets the process id of the process
	* @param process_name the name of the process
	* @return the process id of the process
	*/
	DWORD GetPidFromName(std::string process_name);

	/**
	* brief Gets all the processes id(s) of the process
	* @param process_name the name of the process
	* @returns all the processes id(s) of the process
	*/
	std::vector<int> GetPidListFromName(std::string process_name);

	/**
	* \brief Gets the module list of the process
	* \param process_name the name of the process
	* \return all the module names of the process
	*/
	std::vector<std::string> GetModuleList(std::string process_name);

	/**
	* \brief Gets the process information
	* \return the process information
	*/
	VMMDLL_PROCESS_INFORMATION GetProcessInformation();

	/**
	* \brief Gets the process peb
	* \return the process peb
	*/
	PEB GetProcessPeb();

	/**
	* brief Gets the base address of the process
	* @param module_name the name of the module
	* @return the base address of the process
	*/
	size_t GetBaseDaddy(std::string module_name);

	/**
	* brief Gets the base size of the process
	* @param module_name the name of the module
	* @return the base size of the process
	*/
	size_t GetBaseSize(std::string module_name);

	/**
	* brief Gets the export table address of the process
	* @param import the name of the export
	* @param process the name of the process
	* @param module the name of the module that you wanna find the export in
	* @return the export table address of the export
	*/
	uintptr_t GetExportTableAddress(std::string import, std::string process, std::string module);

	/**
	* brief Gets the import table address of the process
	* @param import the name of the import
	* @param process the name of the process
	* @param module the name of the module that you wanna find the import in
	* @return the import table address of the import
	*/
	uintptr_t GetImportTableAddress(std::string import, std::string process, std::string module);

	/**
	 * \brief This fixes the CR3 fuckery that EAC does.
	 * It fixes it by iterating over all DTB's that exist within your system and looks for specific ones
	 * that nolonger have a PID assigned to them, aka their pid is 0
	 * it then puts it in a vector to later try each possible DTB to find the DTB of the process.
	 * NOTE: Using FixCR3 requires you to have symsrv.dll, dbghelp.dll and info.db
	 */
	bool FixCr3();

	/**
	 * \brief Dumps the process memory at address (requires to be a valid PE Header) to the path
	 * \param address the address to the PE Header(BaseAddress)
	 * \param path the path where you wanna save dump to
	 */
	bool DumpMemory(uintptr_t address, std::string path);

	/*This part is where all memory operations are done, such as read, write.*/

	/**
	 * \brief Scans the process for the signature.
	 * \param signature the signature example "48 ? ? ?"
	 * \param range_start Region to start scan from
	 * \param range_end Region up to where it should scan
	 * \param PID (OPTIONAL) where to read to?
	 * \return address of signature
	 */
	uint64_t FindSignature(const char* signature, uint64_t range_start, uint64_t range_end, int PID = 0);

	/**
	 * \brief Writes memory to the process
	 * \param address The address to write to
	 * \param buffer The buffer to write
	 * \param size The size of the buffer
	 * \return
	 */
	bool Write(uintptr_t address, void* buffer, size_t size) const;
	bool Write(uintptr_t address, void* buffer, size_t size, int pid) const;

	/**
	 * \brief Writes memory to the process using a template
	 * \param address to write to
	 * \param value the value you'll write to the address
	 */
	template <typename T>
	void Write(void* address, T value)
	{
		Write(address, &value, sizeof(T));
	}

	template <typename T>
	void Write(uintptr_t address, T value)
	{
		Write(address, &value, sizeof(T));
	}


    /**
     * 从进程中读取内存
     *
     * 此函数旨在进行跨进程的内存读取操作。它尝试从当前进程的指定地址读取特定数量的内存，
     * 并将其存储在提供的缓冲区中。常用于调试或内存检查工具。
     *
     * @param address 进程内存空间中的起始地址，从此处开始读取。
     * @param buffer 指向缓冲区的指针，读取的内存数据将被存储在此处。
     * @param size 要读取的字节数。
     * @return 如果内存读取操作成功，则返回true；如果发生错误（如地址无效或读取权限不足），则返回false。
     */
    bool Read(uintptr_t address, void* buffer, size_t size) const;

	bool Read(uintptr_t address, void* buffer, size_t size, int pid) const;

	/**
	* brief Reads memory from the process using a template
	* @param address The address to read from
	* @return the value read from the process
	*/
	template <typename T>
	T Read(void* address)
	{
		T buffer { };
		memset(&buffer, 0, sizeof(T));
		Read(reinterpret_cast<uint64_t>(address), reinterpret_cast<void*>(&buffer), sizeof(T));

		return buffer;
	}

	template <typename T>
	T Read(uint64_t address)
	{
		return Read<T>(reinterpret_cast<void*>(address));
	}

	/**
	* brief Reads memory from the process using a template and pid
	* @param address The address to read from
	* @param pid The process id of the process
	* @return the value read from the process
	*/
	template <typename T>
	T Read(void* address, int pid)
	{
		T buffer { };
		memset(&buffer, 0, sizeof(T));
		Read(reinterpret_cast<uint64_t>(address), reinterpret_cast<void*>(&buffer), sizeof(T), pid);

		return buffer;
	}

	template <typename T>
	T Read(uint64_t address, int pid)
	{
		return Read<T>(reinterpret_cast<void*>(address), pid);
	}

		/**
	 * @brief 从指定的基础地址开始读取偏移量链，以获取最终值
	 * @param base 起始读取地址
	 * @param offsets 一系列偏移量值，按顺序使用以计算下一次读取的地址
	 * @return 经过偏移量链遍历后读取到的值
	 *
	 * 该函数首先读取基础地址加上第一个偏移量所得地址处的值，
	 * 然后使用这个值作为新的基础地址，加上第二个偏移量读取下一个值，以此类推。
	 * 它主要用于数据以链式或链接结构存储的场景中，需要根据当前数据计算下一块数据的地址。
	 */
	uint64_t ReadChain(uint64_t base, const std::vector<uint64_t>& offsets)
	{
		// 从基础地址加上第一个偏移量的位置读取第一块数据
		uint64_t result = Read<uint64_t>(base + offsets.at(0));
		// 从第二个偏移量开始，依次使用之前读取的数据作为基础地址读取后续数据
		for (int i = 1; i < offsets.size(); i++)
		{
			result = Read<uint64_t>(result + offsets.at(i));
		}
		// 返回最终读取到的值
		return result;
	}


	/**
	 * \brief Create a scatter handle, this is used for scatter read/write requests
	 * \return Scatter handle
	 */
	VMMDLL_SCATTER_HANDLE CreateScatterHandle() const;
	VMMDLL_SCATTER_HANDLE CreateScatterHandle(int pid) const;

	/**
	 * \brief Closes the scatter handle
	 * \param handle
	 */
	void CloseScatterHandle(VMMDLL_SCATTER_HANDLE handle);

	/**
	 * \brief Adds a scatter read/write request to the handle
	 * \param handle the handle
	 * \param address the address to read/write to
	 * \param buffer the buffer to read/write to
	 * \param size the size of buffer
	 */
	void AddScatterReadRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t address, void* buffer, size_t size);
	void AddScatterWriteRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t address, void* buffer, size_t size);

	/**
	 * \brief Executes all prepared scatter requests, note if you created a scatter handle with a pid
	 * you'll need to specify the pid in the execute function. so we can clear the scatters from the handle.
	 * \param handle
	 * \param pid
	 */
	void ExecuteReadScatter(VMMDLL_SCATTER_HANDLE handle, int pid = 0);
	void ExecuteWriteScatter(VMMDLL_SCATTER_HANDLE handle, int pid = 0);

    int readInt(int64_t address){
        return Read<int>(address);
    }
    int64_t readLong(int64_t address){
        return Read<int64_t>(address);
    }
    short readShort(int64_t address){
        return Read<short>(address);
    }
    double readDouble(int64_t address){
        return Read<double>(address);
    }
    BYTE readByte(int64_t address){
        return Read<BYTE>(address);
    }
    std::vector<BYTE> readBytes(int64_t address,int size){
        auto target = std::unique_ptr<BYTE[]>(new uint8_t[size]);
        std::vector<BYTE> result;
        //Read whole modules memory
        Read(address, target.get(), size);
        for (int i = 0; i < size; ++i) {
            result.insert(result.end(), target[i]);
        }
        return result;
    }
	/*the FPGA handle*/
	VMM_HANDLE vHandle;
};

inline Memory mem{
    "DNF.exe",
    true,
    false
};
