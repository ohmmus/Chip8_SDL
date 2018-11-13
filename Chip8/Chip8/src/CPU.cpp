#include "CPU.h"
#include "RAM.h"
#include <SDL.h>
#include <iostream>
#include <cstdlib>

CPU::CPU(RAM * systemRamInstance)
{
	_systemRamInstance = systemRamInstance;

	for (int i = 0; i < 80; i++)
	{
		_systemRamInstance->SetByteSys(i, chip8_fontset[i]);
	}
}

CPU::~CPU()
{
	
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

void CPU::ProcessKeyDown(int key)
{
	if (key == '1')			_keyPress[0x1] = 1;
	else if (key == '2')	_keyPress[0x2] = 1;
	else if (key == '3')	_keyPress[0x3] = 1;
	else if (key == '4')	_keyPress[0xC] = 1;

	else if (key == 'q')	_keyPress[0x4] = 1;
	else if (key == 'w')	_keyPress[0x5] = 1;
	else if (key == 'e')	_keyPress[0x6] = 1;
	else if (key == 'r')	_keyPress[0xD] = 1;

	else if (key == 'a')	_keyPress[0x7] = 1;
	else if (key == 's')	_keyPress[0x8] = 1;
	else if (key == 'd')	_keyPress[0x9] = 1;
	else if (key == 'f')	_keyPress[0xE] = 1;

	else if (key == 'z')	_keyPress[0xA] = 1;
	else if (key == 'x')	_keyPress[0x0] = 1;
	else if (key == 'c')	_keyPress[0xB] = 1;
	else if (key == 'v')	_keyPress[0xF] = 1;

	//printf("Press key %c\n", key);
}

void CPU::ProcessKeyUp(int key)
{
	if (key == '1')			_keyPress[0x1] = 0;
	else if (key == '2')	_keyPress[0x2] = 0;
	else if (key == '3')	_keyPress[0x3] = 0;
	else if (key == '4')	_keyPress[0xC] = 0;

	else if (key == 'q')	_keyPress[0x4] = 0;
	else if (key == 'w')	_keyPress[0x5] = 0;
	else if (key == 'e')	_keyPress[0x6] = 0;
	else if (key == 'r')	_keyPress[0xD] = 0;

	else if (key == 'a')	_keyPress[0x7] = 0;
	else if (key == 's')	_keyPress[0x8] = 0;
	else if (key == 'd')	_keyPress[0x9] = 0;
	else if (key == 'f')	_keyPress[0xE] = 0;

	else if (key == 'z')	_keyPress[0xA] = 0;
	else if (key == 'x')	_keyPress[0x0] = 0;
	else if (key == 'c')	_keyPress[0xB] = 0;
	else if (key == 'v')	_keyPress[0xF] = 0;
}


unsigned short CPU::FetchNextOp()
{
	return _systemRamInstance->GetWordSys(_programCounter);
}

void CPU::Decode(unsigned short opcode)
{
	// Process opcode
	// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
	
	if (opcode != 0xffff)
	{
		//std::cout << "0x" << std::hex << opcode << std::endl;
	}


	switch (opcode & 0xF000)
	{
		case 0x0000:  // 0x00E0 Clear the display.
			switch (opcode & 0x00FF)
			{
				case 0x00E0: ClearScreen(); break;
				case 0x00EE: ReturnFromSubroutine(); break;
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
			std::cout << "Unknown opcode: " << opcode << std::endl;
	}
}

void CPU::ClearScreen()
{
	_systemRamInstance->ClearGFXMem();
	_programCounter += 2;
}

void CPU::ReturnFromSubroutine()
{
	_programCounter = _stack[_stackPointer];
	_stackPointer--;
	_programCounter += 2;
}

void CPU::JumpToLocation(unsigned short opcode)
{
	_programCounter = opcode & 0x0FFF;
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
		_programCounter += 4;
	}
	else
	{
		_programCounter += 2;
	}
}

void CPU::SkipIfRegNotEqualVal(unsigned short opcode)
{
	if ((_registers[(opcode & 0x0f00) >> 8]) != (opcode & 0x00FF))
	{
		_programCounter += 4;
	}
	else
	{
		_programCounter += 2;
	}
}

void CPU::SkipIfRegEqualReg(unsigned short opcode)
{
	if ((_registers[(opcode & 0x0F00) >> 8]) == (_registers[(opcode & 0x00F0) >> 4]))
	{
		_programCounter += 4;
	}
	else
	{
		_programCounter += 2;
	}
}

void CPU::LoadValToReg(unsigned short opcode)
{
	_registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF; 
	_programCounter += 2;
}

void CPU::Add(unsigned short opcode)
{
	unsigned char val = _registers[(opcode & 0x0F00) >> 8];
	val += (opcode & 0x00FF);
	_registers[(opcode & 0x0F00) >> 8] = val;
	_programCounter += 2;
}

void CPU::RegisterOps(unsigned short opcode)
{
	switch (opcode & 0x000F)
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
			_registers[VFIndex] = (_registers[(opcode & 0x0F00) >> 8] > 255) ? 1 : 0;
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] + _registers[(opcode & 0x00F0) >> 4];
			break;
		case 0x0005: // SUB
			_registers[VFIndex] = (_registers[(opcode & 0x0F00) >> 8] > _registers[(opcode & 0x00F0) >> 4]) ?1:0;
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] - _registers[(opcode & 0x00F0) >> 4];
			break; 
		case 0x0006:  // SHR
			_registers[VFIndex] = ((_registers[(opcode & 0x0F00) >> 8] & 0x0001) == 0x0001) ? 1 : 0;
			_registers[(opcode & 0x0F00) >> 8] /= 2;
			break;
		case 0x0007:// SUBN
			_registers[VFIndex] = (_registers[(opcode & 0x0F00) >> 8] < _registers[(opcode & 0x00F0) >> 4]) ? 1 : 0;
			_registers[(opcode & 0x0F00) >> 8] = _registers[(opcode & 0x0F00) >> 8] - _registers[(opcode & 0x00F0) >> 4];
			break;
		case 0x000E: // SHL
			_registers[VFIndex] = _registers[(opcode & 0x0F00) >> 8] >> 7;
			_registers[(opcode & 0x0F00) >> 8] <<=1;
			break;
	}

	_programCounter += 2;
}

void CPU::SkipNextIfNotEqu(unsigned short opcode)
{
	if (_registers[(opcode & 0x0F00) >> 8] != _registers[(opcode & 0x00F0) >> 4])
	{
		_programCounter += 4;
	}
	else
	{
		_programCounter += 2;
	}
}

void CPU::SetIndexRegister(unsigned short opcode)
{
	_indexRegister = opcode & 0x0FFF;
	_programCounter += 2;
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
	_programCounter += 2;
}

void CPU::DisplaySprite(unsigned short opcode)
{
	unsigned short x = _registers[(opcode & 0x0F00) >> 8];
	unsigned short y = _registers[(opcode & 0x00F0) >> 4];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;

	unsigned char * memory = _systemRamInstance->GetSysMem();
	unsigned char * gfx = _systemRamInstance->GetGFXMem();

	_registers[0xF] = 0;
	for (int yline = 0; yline < height; yline++)
	{
		pixel = memory[_indexRegister + yline];
		for (int xline = 0; xline < 8; xline++)
		{
			if ((pixel & (0x80 >> xline)) != 0)
			{
				if (gfx[(x + xline + ((y + yline) * 64))] == 1)
					_registers[0xF] = 1;
				gfx[x + xline + ((y + yline) * 64)] ^= 1;
			}
		}
	}
	
	// TODO: figure out what this doens't work.
	//for (int yline = 0; yline < height; yline++)
	//{
	//	pixel = _systemRamInstance->GetWordSys(_indexRegister + yline);
	//	for (int xline = 0; xline < 8; xline++)
	//	{
	//		if ((pixel & (0x80 >> xline)) != 0)
	//		{
	//			if (_systemRamInstance->GetWordGfx(x + xline + ((y + yline) * 64)) == 1)
	//			{
	//				_registers[0xF] = 1;
	//			}
	//		
	//			unsigned short xoredPixel = _systemRamInstance->GetWordGfx(x + xline + ((y + yline) * 64));
	//			xoredPixel ^= 1;
	//			_systemRamInstance->SetByteGfx(x + xline + ((y + yline) * 64), xoredPixel);
	//		}
	//	}
	//}

	_systemRamInstance->SetDrawFlag(true);
	_programCounter += 2;
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
		_programCounter += 4;
	}
	else
	{
		_programCounter += 2;
	}
}

void CPU::LoadDelayTimerVal(unsigned short opcode)
{
	_registers[(opcode & 0x0F00) >> 8] = (unsigned char)(_delay_timer);
	_programCounter += 2;
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

	_programCounter += 2;
}

void CPU::SetDelayTimerVal(unsigned short opcode)
{
	_delay_timer = _registers[(opcode & 0x0F00) >> 8];
	_programCounter += 2;
}

void CPU::SetSoundTimerVal(unsigned short opcode)
{
	_sound_timer = _registers[(opcode & 0x0F00) >> 8];
	_programCounter += 2;
}

void CPU::AddToIndex(unsigned short opcode)
{
	_indexRegister += _registers[(opcode & 0x0F00) >> 8];

	_registers[0xF] = 0;
	if (_indexRegister > 0xFFF)
	{
		_registers[0xF] = 1;
	}

	_programCounter += 2;
}

void CPU::SetSpriteLoc(unsigned short opcode)
{
	_indexRegister = _registers[(opcode & 0x0F00) >> 8] * 0x5;
	_programCounter += 2;
}

void CPU::StoreBCDRepresentation(unsigned short opcode)
{
	_programCounter += 2;

	unsigned char * memory = _systemRamInstance->GetSysMem();

	memory[_indexRegister] = _registers[(opcode & 0x0F00) >> 8] / 100;
	memory[_indexRegister + 1] = (_registers[(opcode & 0x0F00) >> 8] / 10) % 10;
	memory[_indexRegister + 2] = (_registers[(opcode & 0x0F00) >> 8] % 100) % 10;
}

void CPU::LoadIntoMemory(unsigned short opcode)
{
	unsigned char * memory = _systemRamInstance->GetSysMem();

	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
	{
		memory[_indexRegister + i] = _registers[i];
	}
	
	_programCounter += 2;
}

void CPU::LoadIntoRegisters(unsigned short opcode)
{
	const unsigned char * memory = _systemRamInstance->GetSysMem();

	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
	{
		_registers[i] = memory[_indexRegister + i];
	}

	_programCounter += 2;

}