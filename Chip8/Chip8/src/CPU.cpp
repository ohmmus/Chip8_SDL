#include "CPU.h"
#include "RAM.h"
#include <SDL.h>
#include <iostream>
#include <cstdlib>

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
			std::cout << "BEEP!" << std::endl;
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
	// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
	
	std::cout << opcode << std::endl;

	switch (opcode & 0xF000)
	{
		case 0x0000:  // 0x00E0 Clear the display.
			switch (opcode & 0x000F)
			{
				case 0x0000: ClearScreen(); break;
				case 0x000E: ReturnFromSubroutine(); break;
			}
			break;

		// 0x1NNN: Jumps to address NNN
		case 0x1000: JumpToLocation(opcode); break;
		
		// 0x2NNN: Calls subroutine at NNN.
		case 0x2000: CallSubroutineAtLocation(opcode); break;

		// 0x3XNN: Skips the next instruction if VX equals NN
		case 0x3000: SkipIfRegEqualVal(opcode);  break;
			
		// 0x4XNN: Skips the next instruction if VX doesn't equal NN
		case 0x4000: SkipIfRegNotEqualVal(opcode); break;

		// 0x5XY0: Skips the next instruction if VX equals VY.
		case 0x5000: SkipIfRegEqualReg(opcode); break;

		// 0x6XNN: Sets VX to NN.
		case 0x6000: LoadValToReg(opcode); break;

		// 0x7XNN: Adds NN to VX.
		case 0x7000: Add(opcode); break;

		// Arithmetic and Logical Register ops. 
		case 0x8000: RegisterOps(opcode); break;

		// 0x9XY0: Skips the next instruction if VX doesn't equal VY
		case 0x9000: SkipNextIfNotEqu(opcode); break;

		// ANNN: Sets I to the address NNN
		case 0xA000: SetIndexRegister(opcode); break;

		// BNNN: Jumps to the address NNN plus V0
		case 0xB000: Jump(opcode); break;

		// CXNN: Sets VX to a random number and NN
		case 0xC000: SetRandWithValue(opcode); break;
		case 0xD000: DisplaySprite(opcode); break;
		case 0xE000: SkipNextInstIf(opcode); break;			
		case 0xF000: 
			switch (opcode & 0x00FF)
			{
				case 0x0007: LoadDelayTimerVal(opcode); break;
				case 0x000A: WaitForKeyPress(opcode); break;
				case 0x0015: SetDelayTimerVal(opcode); break;
				case 0x0018: SetSoundTimerVal(opcode); break;
				case 0x001E: AddToIndex(opcode); break;
				case 0x0029: SetSpriteLoc(opcode); break;
				case 0x0033: StoreBCDRepresentation(opcode); break;
				case 0x0055: LoadIntoMemory(opcode); break;
				case 0x0065: LoadIntoRegisters(opcode); break;
			}
			break;

		default:
			printf("Unknown opcode: 0x%X\n", opcode);
	}
}

void CPU::ClearScreen()
{
	_systemRam->ClearGFXMem();
	_programCounter += 2;
}

void CPU::ReturnFromSubroutine()
{
	_programCounter = _stack[_stackPointer];
	_stackPointer--;
}

void CPU::JumpToLocation(unsigned short opcode)
{
	_programCounter = opcode & 0x0FFF;
	_programCounter++;
}

void CPU::CallSubroutineAtLocation(unsigned short opcode)
{
	_stackPointer++;
	_stack[_stackPointer] = _programCounter;
	_programCounter = opcode & 0x0FFF;
}

void CPU::SkipIfRegEqualVal(unsigned short opcode)
{
	if ((_registers[(opcode & 0x0f00) >> 8]) == (opcode & 0x00FF))
	{
		_programCounter += 2;
	}
}

void CPU::SkipIfRegNotEqualVal(unsigned short opcode)
{
	if ((_registers[(opcode & 0x0f00) >> 8]) != (opcode & 0x00FF))
	{
		_programCounter += 2;
	}
}

void CPU::SkipIfRegEqualReg(unsigned short opcode)
{
	if ((_registers[(opcode & 0x0F00) >> 8]) == (_registers[(opcode & 0x00F0) >> 4]))
	{
		_programCounter += 2;
	}
}

void CPU::LoadValToReg(unsigned short opcode)
{
	_registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF; 
	_programCounter++;
}

void CPU::Add(unsigned short opcode)
{
	unsigned char val = _registers[(opcode & 0x0F00) >> 8];
	val += (opcode & 0x00FF);
	_registers[(opcode & 0x0F00) >> 8] = val;
	_programCounter++;
}

void CPU::RegisterOps(unsigned short opcode)
{
	switch (opcode * 0x000F)
	{
		case 0x0000:  // LD
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x00F0) >> 4];
		break;
		case 0x0001: // OR
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] | _registers[(opcode & 0x00F0) >> 4];
			break;
		case 0x0002: // AND
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] & _registers[(opcode & 0x00F0) >> 4];
			break;
		case 0x0003: // XOR
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] ^ _registers[(opcode & 0x00F0) >> 4];
			break;
		case 0x0004: // ADD
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] + _registers[(opcode & 0x00F0) >> 4];
			_registers[VFIndex] = (_registers[(opcode & 0x0F00) >> 8] > 255) ? 1 : 0;
			break;
		case 0x0005: // SUB
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] - _registers[(opcode & 0x00F0) >> 4];
			_registers[VFIndex] = (_registers[(opcode & 0x0F00) >> 8] > _registers[(opcode & 0x00F0) >> 4]) ? 1 : 0;
			break; 
		case 0x0006:  // SHR
			_registers[(opcode & 0x0F00) >> 8] /= 2;
			_registers[VFIndex] = ((_registers[(opcode & 0x0F00) >> 8] & 0x0001) == 0x0001) ? 1 : 0;
			break;
		case 0x0007:// SUBN
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] - _registers[(opcode & 0x00F0) >> 4];
			_registers[VFIndex] = (_registers[(opcode & 0x0F00) >> 8] < _registers[(opcode & 0x00F0) >> 4]) ? 1 : 0;
			break;
		case 0x000E: // SHL
			_registers[VFIndex] = ((_registers[(opcode & 0x0F00) >> 8] & 0x8000) == 0x8000) ? 1 : 0;
			_registers[(opcode & 0x0F00) >> 8] *= 2;
			break;
	}

	_programCounter++;
}

void CPU::SkipNextIfNotEqu(unsigned short opcode)
{
	if (_registers[(opcode & 0x0F00) >> 8] != _registers[(opcode & 0x00F0) >> 4])
	{
		_programCounter += 2;
	}
}

void CPU::SetIndexRegister(unsigned short opcode)
{
	_indexRegister = opcode & 0x0FFF;
	_programCounter++;
}

void CPU::Jump(unsigned short opcode)
{
	_programCounter = (opcode * 0x0FFF) + _registers[0];
}

void CPU::SetRandWithValue(unsigned short opcode)
{
	unsigned char random = (unsigned char)(rand() % 255);
	unsigned char value = opcode & 0x00FF & random;
	_registers[(opcode & 0x0F00) >> 8] = value;
	_programCounter++;
}

void CPU::DisplaySprite(unsigned short opcode)
{
	// TODO: 
	_programCounter++;
}

void CPU::SkipNextInstIf(unsigned short opcode)
{
	unsigned char regVal = opcode & 0x0F00 >> 8;
	bool doSkip = false;

	switch (opcode & 0xF0FF)
	{
		case 0xE09E: doSkip = _keyPress[regVal] == 1 ? true : false; break;
		case 0xE0A1: doSkip = _keyPress[regVal] == 0 ? true : false; break;
	}

	if (doSkip)
	{
		_programCounter += 2;
	}

}

void CPU::LoadDelayTimerVal(unsigned short opcode)
{
	_registers[(opcode & 0x0F00) >> 8] = (unsigned char)(_delay_timer);
	_programCounter++;
}

void CPU::WaitForKeyPress(unsigned short opcode)
{
	// TODO: stop execution. 

	if (_keyPress[0] || 
		_keyPress[1] ||
		_keyPress[2] ||
		_keyPress[3] ||
		_keyPress[4] ||
		_keyPress[5] ||
		_keyPress[6] ||
		_keyPress[7] ||
		_keyPress[8] ||
		_keyPress[9] ||
		_keyPress[10] ||
		_keyPress[11] ||
		_keyPress[12] ||
		_keyPress[13] ||
		_keyPress[14] ||
		_keyPress[15])
	{
		for (int i = 0; i < VFIndex; i++)
		{
			if (_keyPress[i] == 1)
			{
				_registers[(opcode & 0x0F00) >> 8] = i;

				// TODO: resume execution;

				break;
			}
		}
	}

	_programCounter++;
}

void CPU::SetDelayTimerVal(unsigned short opcode)
{
	_delay_timer = _registers[(opcode & 0x0F00) >> 8];
	_programCounter++;
}

void CPU::SetSoundTimerVal(unsigned short opcode)
{
	_sound_timer = _registers[(opcode & 0x0F00) >> 8];
	_programCounter++;
}

void CPU::AddToIndex(unsigned short opcode)
{
	_indexRegister += _registers[(opcode & 0x0F00) >> 8];
	_programCounter++;
}

void CPU::SetSpriteLoc(unsigned short opcode)
{
	// TODO: 
	_programCounter++;
}

void CPU::StoreBCDRepresentation(unsigned short opcode)
{
	// TODO:
	_programCounter++;
}

void CPU::LoadIntoMemory(unsigned short opcode)
{
	// TODO:
	_programCounter++;
}

void CPU::LoadIntoRegisters(unsigned short opcode)
{
	// TODO:
	_programCounter++;
}