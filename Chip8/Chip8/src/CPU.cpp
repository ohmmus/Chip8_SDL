#include "CPU.h"
#include "RAM.h"
#include <SDL.h>
#include <iostream>

CPU::CPU()
{
	_systemRam = new RAM;

	// TEST ONLY
	_systemRam->LoadROM("./res/roms/PONG");
}

CPU::~CPU()
{
	delete _systemRam;
}

void CPU::Initialize()
{
	_programCounter = 0x200;
	_indexRegister = 0;
	_stackPointer = 0;
	_delay_timer = 0;
	_sound_timer = 0;
}

void CPU::Emulate()
{
	// Fetch
	unsigned short nextOp = FetchNextOp();

	// Decode
	Decode(nextOp);

	if (_delay_timer > 0)
	{ 
		_delay_timer--;
	}

	if (_sound_timer > 0)
	{
		if (_sound_timer == 1)
		{
			//std::cout << "BEEP!" << std::endl;
		}
		_sound_timer--;
	}
}

unsigned short CPU::FetchNextOp()
{
	return _systemRam->GetWordSys(_programCounter);
}

void CPU::Decode(unsigned short opcode)
{
	// Process opcode
	// http://www.multigesture.net/wp-content/uploads/mirror/goldroad/chip8_instruction_set.shtml
	switch (opcode & 0xF000)
	{
	case 0x0000: // Unused in modern interpreters
		
		break;

	case 0x1000: // 0x1NNN: Jumps to address NNN
		
		break;

	case 0x2000: // 0x2NNN: Calls subroutine at NNN.
		
		break;

	case 0x3000: // 0x3XNN: Skips the next instruction if VX equals NN
		
		break;

	case 0x4000: // 0x4XNN: Skips the next instruction if VX doesn't equal NN
		
		break;

	case 0x5000: // 0x5XY0: Skips the next instruction if VX equals VY.
		
		break;

	case 0x6000: // 0x6XNN: Sets VX to NN.
		
		break;

	case 0x7000: // 0x7XNN: Adds NN to VX.
		
		break;

	case 0x8000:
	
		break;

	case 0x9000: // 0x9XY0: Skips the next instruction if VX doesn't equal VY
	
		break;

	case 0xA000: // ANNN: Sets I to the address NNN
	
		break;

	case 0xB000: // BNNN: Jumps to the address NNN plus V0
	
		break;

	case 0xC000: // CXNN: Sets VX to a random number and NN
	
		break;

	case 0xD000: 
	
		break;

	case 0xE000:
		
		break;

	case 0xF000:
		
		break;

	default:
		printf("Unknown opcode: 0x%X\n", opcode);
	}
}


