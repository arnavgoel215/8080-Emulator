#include "memory.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

// ================= Constructor ============================ 
//Initalize and clear the memory on startup
Memory::Memory() {
    mem.fill(0x00);  
// ------ DEBUG MODE ------
// Initialize debug tracking counters and snapshot
#ifdef ENABLE_MEMORY_DEBUG
    snapshot.clear();
    readCounts.clear();
    writeCounts.clear();
    watchpoints.clear();
#endif // --- END DEBUG --- 
}

//================== Core Memory Access ======================

// -------------------- READ ------------------------------
// === READ === Handles Memory read requests
uint8_t Memory::ReadByte(uint16_t address) const {

 // --- DEBUG MODE --- Track Memory Reads and Check for a watchpoint   
#ifdef ENABLE_MEMORY_DEBUG
    ++readCounts[address];
    if (watchpoints.find(address) != watchpoints.end()) {
        std::cout << "[Watchpoint] READ at 0x" 
          << std::hex << std::setw(4) << std::setfill('0') << address
          << ": 0x" << std::setw(2) << (int)mem[address] << "\n";
    }
#endif // --- END DEBUG ---
    return mem[address]; // Send memory Bytes 
}

// ------------------ Write: ROM ONLY  -------------------------
// === ROM WRITE: (ONLY FOR ROMLoader) ===  Writes to the ROM section of RAM
// Bypasses the memory ROM protection
void Memory::writeRomBytes(uint16_t address, uint8_t value) {
    if (address < 0x2000) {
        mem[address] = value;
    
    // --- DEBUG MODE --- 
    #ifdef ENABLE_MEMORY_DEBUG
   /* if (address < 0x2000) {
        std::cout << "[ROM byte] " << std::hex << std::setw(4) << std::setfill('0') << address
                << " = " << std::setw(2) << static_cast<int>(value) << "\n";
    }*/
    #endif // --- END DEBUG ---
    } else {
        // Error catching: if ROM writes outside of 0x2000
        std::cerr << "[Error] writeRomBytes() attempted to write outside ROM at 0x"
                  << std::hex << std::setw(4) << std::setfill('0') << address << "\n";

    }
}

// ----------------- Write (Normal) --------------------------

// === Core Write == Handles Memory normal writing request
void Memory::WriteByte(uint16_t address, uint8_t value) {
   // Blocks writing to ROM section
    if (address < 0x2000) {

// --- DEBUG MODE --- Display Warning when trying to write to ROM (< 0x2000)
#ifdef ENABLE_MEMORY_DEBUG
        std::cout << "[Warning] Attempted to write to ROM at 0x"
                  << std::hex << std::setw(4) << std::setfill('0') << address
                  << " ignored.\n";
#endif // --- END DEBUG --- 
        return; // Blocks ROM writes 
    }

// --- DEBUG MODE --- Track Memory writes and Check for a watchpoint  
#ifdef ENABLE_MEMORY_DEBUG
    ++writeCounts[address];
    // Display watchpoint value
    if (watchpoints.find(address) != watchpoints.end()) {
       std::cout << "[Watchpoint] WRITE at 0x"
                 << std::hex << std::setw(4) << std::setfill('0') << address
                 << ": 0x" << std::setw(2) << static_cast<int>(value) << "\n";
    }
#endif // --- END DEBUG ---
    // Write to memory at specific address
    mem[address] = value; 
}


// ----------------- Video RAM Access -------------------------------

// VRAM boundaries 
static constexpr uint16_t VRAM_START = 0x2400;
static constexpr uint16_t VRAM_END   = 0x3FFF;

// === VRAM ===  Makes a copy video memory for Qt rendering (Framebuffer)
std::vector<uint8_t> Memory::GetVRAM() const {
    std::vector<uint8_t> vram;
    vram.reserve(VRAM_END - VRAM_START + 1);

    for (uint16_t addr = VRAM_START; addr <= VRAM_END; ++addr) {
        vram.push_back(mem[addr]);
    }

    return vram;
}

// === VRAM === Direct read-only pointer to video RAM
const uint8_t* Memory::GetVRAMPointer() const {
    return &mem[VRAM_START];
}

// --- DEBUG MODE -- 
#ifdef ENABLE_MEMORY_DEBUG
// Displays VRAM on console
void Memory::DumpVRAM(uint16_t start, uint16_t end) const {
    if (start < VRAM_START || end > VRAM_END || start > end) {
        std::cerr << "[Error] Invalid VRAM dump range: 0x"
                  << std::hex << std::setw(4) << std::setfill('0') << start
                  << " to 0x" << std::setw(4) << end << "\n";
        return;
    }

    for (uint16_t addr = start; addr <= end; ++addr) {
        std::cout << "VRAM[0x" << std::hex << std::setw(4) << std::setfill('0') << addr
                  << "] = 0x" << std::setw(2) << static_cast<int>(mem[addr]) << "\n";
    }
}
#endif // --- END DEBUG ---


// ======================== Debug Tools ==============================
#ifdef ENABLE_MEMORY_DEBUG

// === Debug: Full Memory Dump ===
void Memory::DumpMemory(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "[Error] Failed to open \"" << filename << "\" for memory dump.\n";
        return;
    }

    out.write(reinterpret_cast<const char*>(mem.data()), MEMORY_SIZE);
    out.close();
    std::cout << "[Debug] Memory dumped to \"" << filename << "\"\n";
}

// === Debug: Dump Region of Memory ===
void Memory::DumpRegion(uint16_t start, uint16_t end) const {
    for (uint16_t addr = start; addr <= end; ++addr) {
        if ((addr - start) % 16 == 0)
            std::cout << "\n0x" << std::hex << std::setw(4) << std::setfill('0') << addr << ": ";

        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(mem[addr]) << " ";
    }
    std::cout << std::dec << "\n";
}

// === Debug: Take a Snapshot of Memory ===
void Memory::Snapshot() {
    snapshot.assign(mem.begin(), mem.end());  
    std::cout << "[Debug] Snapshot taken.\n";
}

// === Debug: Compare Snapshot to Memory ===
void Memory::CompareWithSnapshot() const {
    std::cout << "[Debug] Comparing memory with snapshot:\n";
    bool found = false;

    for (size_t i = 0; i < MEMORY_SIZE; ++i) {
        if (mem[i] != snapshot[i]) {
            std::cout << "  0x" << std::hex << std::setw(4) << std::setfill('0') << i
                      << ": was 0x" << std::setw(2) << static_cast<int>(snapshot[i])
                      << ", now 0x" << std::setw(2) << static_cast<int>(mem[i]) << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "  No changes detected.\n";
    }
}

// === Debug: Create Watchpoints in Memory ===
void Memory::AddWatchpoint(uint16_t address) {
    watchpoints.insert(address);
    std::cout << "[Debug] Watchpoint added at 0x"
              << std::hex << std::setw(4) << std::setfill('0') << address << "\n";
}
// === Debug: Clear Watchpoints in Memory ===
void Memory::ClearWatchpoints() {
    watchpoints.clear();
    std::cout << "[Debug] All watchpoints cleared.\n";
}

// === Debug: Access Logging for Reads and Writes ===
void Memory::LogAccessCounts(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[Error] Failed to open \"" << filename << "\" for access logging.\n";
        return;
    }

    out << "Address\tReads\tWrites\n";
    for (size_t i = 0; i < MEMORY_SIZE; ++i) {
        uint16_t addr = static_cast<uint16_t>(i);
        int reads = readCounts.count(addr) ? readCounts.at(addr) : 0;
        int writes = writeCounts.count(addr) ? writeCounts.at(addr) : 0;

        if (reads > 0 || writes > 0) {
            out << "0x" << std::hex << std::setw(4) << std::setfill('0') << addr
                << "\t" << std::dec << reads << "\t" << writes << "\n";
        }
    }

    out.close();
    std::cout << "[Debug] Access counts logged to \"" << filename << "\"\n";
}

#endif // --- END DEBUG TOOLS --- 

