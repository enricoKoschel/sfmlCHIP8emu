#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>
#include "sound.h"
#include <windows.h>

using namespace std;

// screen
const int screenWidth = 64, screenHeight = 32;
const int scale = 32;
sf::RectangleShape screen[screenWidth * screenHeight];

string game = "";

void initializeScreen() {
	for (int i = 0; i < screenWidth * screenHeight; i++) {
		screen[i] = sf::RectangleShape(sf::Vector2f(scale, scale));
		int x = i % screenWidth * scale;
		int y = i / screenWidth * scale;
		screen[i].setPosition(x, y);
		screen[i].setFillColor(sf::Color::Black);
	}
}

int handleArguments(int argc, char* argv[]) {
	if (argc == 2) game = argv[1];
	else return 1;
	return 0;
}

class chip8 {
private:
	typedef unsigned char BYTE;
	typedef unsigned short WORD;

	BYTE memory[0xFFF];
	BYTE registers[16];
	WORD I;
	WORD PC;
	vector<WORD> stack;
	BYTE delayTimer = 0;
	BYTE soundTimer = 0;

	WORD fetchOpcode() {
		BYTE msb = memory[PC++];
		BYTE lsb = memory[PC++];
		return (msb << 8) | lsb;
	}

	void decodeOpcode() {
		WORD opcode = fetchOpcode();

		switch (opcode & 0xF000) {
		case 0x0000:
			switch (opcode & 0x000F) {
			case 0x0000:
				//00E0
				op00E0();
				break;
			case 0x000E:
				//00EE
				op00EE();
				break;
			}
			break;
		case 0x1000:
			//1NNN
			op1NNN(opcode);
			break;
		case 0x2000:
			//2NNN
			op2NNN(opcode);
			break;
		case 0x3000:
			//3XNN
			op3XNN(opcode);
			break;
		case 0x4000:
			//4XNN
			op4XNN(opcode);
			break;
		case 0x5000:
			//5XY0
			op5XY0(opcode);
			break;
		case 0x6000:
			//6XNN
			op6XNN(opcode);
			break;
		case 0x7000:
			//7XNN
			op7XNN(opcode);
			break;
		case 0x8000:
			switch (opcode & 0x000F) {
			case 0x0000:
				//8XY0
				op8XY0(opcode);
				break;
			case 0x0001:
				//8XY1
				op8XY1(opcode);
				break;
			case 0x0002:
				//8XY2
				op8XY2(opcode);
				break;
			case 0x0003:
				//8XY3
				op8XY3(opcode);
				break;
			case 0x0004:
				//8XY4
				op8XY4(opcode);
				break;
			case 0x0005:
				//8XY5
				op8XY5(opcode);
				break;
			case 0x0006:
				//8XY6
				op8XY6(opcode);
				break;
			case 0x0007:
				//8XY7
				op8XY7(opcode);
				break;
			case 0x000E:
				//8XYE
				op8XYE(opcode);
				break;
			}
			break;
		case 0x9000:
			//9XY0
			op9XY0(opcode);
			break;
		case 0xA000:
			//ANNN
			opANNN(opcode);
			break;
		case 0xB000:
			//BNNN
			opBNNN(opcode);
			break;
		case 0xC000:
			//CXNN
			opCXNN(opcode);
			break;
		case 0xD000:
			//DXYN
			opDXYN(opcode);
			break;
		case 0xE000:
			switch (opcode & 0x00F0) {
			case 0x0090:
				//EX9E
				opEX9E(opcode);
				break;
			case 0x00A0:
				//EXA1
				opEXA1(opcode);
				break;
			}
			break;
		case 0xF000:
			switch (opcode & 0x00FF) {
			case 0x0007:
				//FX07
				opFX07(opcode);
				break;
			case 0x000A:
				//FX0A
				opFX0A(opcode);
				break;
			case 0x0015:
				//FX15
				opFX15(opcode);
				break;
			case 0x0018:
				//FX18
				opFX18(opcode);
				break;
			case 0x001E:
				//FX1E
				opFX1E(opcode);
				break;
			case 0x0029:
				//FX29
				opFX29(opcode);
				break;
			case 0x0033:
				//FX33
				opFX33(opcode);
				break;
			case 0x0055:
				//FX55
				opFX55(opcode);
				break;
			case 0x0065:
				//FX65
				opFX65(opcode);
				break;
			}
			break;
		}
	}

	WORD getKeysPressed() {
		WORD keys = 0;
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad0) ? 1 : 0) << 0; // 0 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1) ? 1 : 0) << 1; // 1 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2) ? 1 : 0) << 2; // 2 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad3) ? 1 : 0) << 3; // 3 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad4) ? 1 : 0) << 4; // 4 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad5) ? 1 : 0) << 5; // 5 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad6) ? 1 : 0) << 6; // 6 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad7) ? 1 : 0) << 7; // 7 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8) ? 1 : 0) << 8; // 8 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad9) ? 1 : 0) << 9; // 9 KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ? 1 : 0) << 10; // A KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B) ? 1 : 0) << 11; // B KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C) ? 1 : 0) << 12; // C KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ? 1 : 0) << 13; // D KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) ? 1 : 0) << 14; // E KEY
		keys |= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F) ? 1 : 0) << 15; // F KEY

		return keys;
	}

	//All opcodes except 0NNN
	void op00E0() {
		// Clears the screen
		for (int i = 0; i < screenWidth * screenHeight; i++) {
			screen[i].setFillColor(sf::Color::Black);
		}
	}
	void op00EE() {
		// Returns from a subroutine
		PC = stack.back();
		stack.pop_back();
	}
	void op1NNN(WORD opcode) {
		// Jumps to address NNN
		PC = opcode & 0x0FFF;
	}
	void op2NNN(WORD opcode) {
		// Calls subroutine at NNN
		stack.push_back(PC);
		PC = opcode & 0x0FFF;
	}
	void op3XNN(WORD opcode) {
		// Skips the next instruction if VX equals NN
		if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
			PC += 2;
		}
	}
	void op4XNN(WORD opcode) {
		// Skips the next instruction if VX doesn't equal NN
		if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
			PC += 2;
		}
	}
	void op5XY0(WORD opcode) {
		// Skips the next instruction if VX equals VY
		if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4]) {
			PC += 2;
		}
	}
	void op6XNN(WORD opcode) {
		// Sets VX to NN
		registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
	}
	void op7XNN(WORD opcode) {
		// Adds NN to VX
		registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
	}
	void op8XY0(WORD opcode) {
		// Sets VX to the value of VY
		registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
	}
	void op8XY1(WORD opcode) {
		// Sets VX to VX bitwise or VY
		registers[(opcode & 0x0F00) >> 8] |= registers[(opcode & 0x00F0) >> 4];
	}
	void op8XY2(WORD opcode) {
		// Sets VX to VX bitwise and VY
		registers[(opcode & 0x0F00) >> 8] &= registers[(opcode & 0x00F0) >> 4];
	}
	void op8XY3(WORD opcode) {
		// Sets VX to VX xor VY
		registers[(opcode & 0x0F00) >> 8] ^= registers[(opcode & 0x00F0) >> 4];
	}
	void op8XY4(WORD opcode) {
		// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
		registers[(opcode & 0x0F00) >> 8] += registers[(opcode & 0x00F0) >> 4];

		registers[0xF] = (registers[(opcode & 0x0F00) >> 8] + registers[(opcode & 0x00F0) >> 4] > 255) ? 1 : 0;
	}
	void op8XY5(WORD opcode) {
		// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
		registers[(opcode & 0x0F00) >> 8] -= registers[(opcode & 0x00F0) >> 4];

		registers[0xF] = ((registers[(opcode & 0x0F00) >> 8] - registers[(opcode & 0x00F0) >> 4]) < 0) ? 1 : 0;
	}
	void op8XY6(WORD opcode) {
		// Stores the least significant bit of VX in VF and then shifts VX to the right by 1
		registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0b00000001;
		registers[(opcode & 0x0F00) >> 8] >>= 1;
	}
	void op8XY7(WORD opcode) {
		// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
		registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8];

		registers[0xF] = ((registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8]) < 0) ? 0 : 1;
	}
	void op8XYE(WORD opcode) {
		// Stores the most significant bit of VX in VF and then shifts VX to the left by 1
		registers[0xF] = (registers[(opcode & 0x0F00) >> 8] & 0b10000000) >> 7;
		registers[(opcode & 0x0F00) >> 8] <<= 1;
	}
	void op9XY0(WORD opcode) {
		// Skips the next instruction if VX doesn't equal VY
		if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4]) {
			PC += 2;
		}
	}
	void opANNN(WORD opcode) {
		// Sets I to the address NNN
		I = opcode & 0x0FFF;
	}
	void opBNNN(WORD opcode) {
		// Jumps to the address NNN plus V0
		PC = (opcode & 0x0FFF) + registers[0];
	}
	void opCXNN(WORD opcode) {
		// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
		registers[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
	}
	void opDXYN(WORD opcode) {
		// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
		// Each row of 8 pixels is read as bit-coded starting from memory location I
		// I value doesn’t change after the execution of this instruction
		// VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, 
		// and to 0 if that doesn’t happen

		// screen index
		int width = screenWidth;
		int row = registers[(opcode & 0x00F0) >> 4];
		int col = registers[(opcode & 0x0F00) >> 8];
		int index = width * row + col;

		for (int i = 0; i < (opcode & 0x000F); i++) {
			for (int j = 0; j < 8; j++) {
				int pixel = index + j + (i * width);
				if ((screen[pixel].getFillColor() == sf::Color::White && ((memory[I + i] << j) & 0b10000000))) {
					screen[pixel].setFillColor(sf::Color::Black);
					registers[0xF] = 1;
				}
				else if (screen[pixel].getFillColor() == sf::Color::Black && ((memory[I + i] << j) & 0b10000000)) {
					screen[pixel].setFillColor(sf::Color::White);;
					registers[0xF] = 0;
				}
			}
		}
	}
	void opEX9E(WORD opcode) {
		// Skips the next instruction if the key stored in VX is pressed
		BYTE x = registers[(opcode & 0x0F00) >> 8];
		WORD key = (getKeysPressed() >> x) & 0x1;
		if (key) {
			PC += 2;
		}
	}
	void opEXA1(WORD opcode) {
		// Skips the next instruction if the key stored in VX isn't pressed
		BYTE x = registers[(opcode & 0x0F00) >> 8];
		WORD key = (getKeysPressed() >> x) & 0x1;
		if (!key) {
			PC += 2;
		}
	}
	void opFX07(WORD opcode) {
		// Sets VX to the value of the delay timer
		registers[(opcode & 0x0F00) >> 8] = delayTimer;
	}
	void opFX0A(WORD opcode) {
		// A key press is awaited, and then stored in VX
		WORD key = getKeysPressed();
		while (!key) {
			key = getKeysPressed();
		}
		for (int i = 0; i < 16; i++) {
			if ((key >>= i) && 0x1) {
				registers[(opcode & 0x0F00) >> 8] = i + 1;
				break;
			}
		}
	}
	void opFX15(WORD opcode) {
		// Sets the delay timer to VX
		delayTimer = registers[(opcode & 0x0F00) >> 8];
	}
	void opFX18(WORD opcode) {
		// 	Sets the sound timer to VX
		soundTimer = registers[(opcode & 0x0F00) >> 8];
	}
	void opFX1E(WORD opcode) {
		// Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't
		I += registers[(opcode & 0x0F00) >> 8];
		registers[0xF] = (I + registers[(opcode & 0x0F00) >> 8]) > 0xFFF ? 1 : 0;
	}
	void opFX29(WORD opcode) {
		// Sets I to the location of the sprite for the character in VX
		// Characters 0-F (in hexadecimal) are represented by a 4x5 font
		I = registers[(opcode & 0x0F00) >> 8] * 5;
	}
	void opFX33(WORD opcode) {
		// Stores the binary-coded decimal representation of VX, 
		// with the most significant of three digits at the address in I, 
		// the middle digit at I plus 1, and the least significant digit at I plus 2
		int value = registers[(opcode & 0x0F00) >> 8];
		memory[I] = value / 100;
		memory[I + 1] = (value / 10) % 10;
		memory[I + 2] = value % 10;
	}
	void opFX55(WORD opcode) {
		// Stores V0 to VX (including VX) in memory starting at address I
		for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
			memory[I + i] = registers[i];
		}
	}
	void opFX65(WORD opcode) {
		// Fills V0 to VX (including VX) with values from memory starting at address I
		for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
			registers[i] = memory[I + i];
		}
	}
public:
	void resetCPU() {
		// reset registers
		for (int i = 0; i < 16; i++) {
			registers[i] = 0;
		}
		I = 0;
		PC = 0x200;
		stack.clear();

		//reset timers
		delayTimer = 0;
		soundTimer = 0;

		// load font into memory
		// 0
		memory[0] = 0b01100000;
		memory[1] = 0b11010000;
		memory[2] = 0b10010000;
		memory[3] = 0b10110000;
		memory[4] = 0b01100000;
		// 1
		memory[5] = 0b00100000;
		memory[6] = 0b01100000;
		memory[7] = 0b00100000;
		memory[8] = 0b00100000;
		memory[9] = 0b01110000;
		// 2
		memory[10] = 0b01100000;
		memory[11] = 0b10010000;
		memory[12] = 0b00100000;
		memory[13] = 0b01000000;
		memory[14] = 0b11110000;
		// 3
		memory[15] = 0b01100000;
		memory[16] = 0b10010000;
		memory[17] = 0b00100000;
		memory[18] = 0b10010000;
		memory[19] = 0b01100000;
		// 4
		memory[20] = 0b00100000;
		memory[21] = 0b01100000;
		memory[22] = 0b10100000;
		memory[23] = 0b11110000;
		memory[24] = 0b00100000;
		// 5
		memory[25] = 0b11110000;
		memory[26] = 0b10000000;
		memory[27] = 0b11100000;
		memory[28] = 0b00010000;
		memory[29] = 0b11100000;
		// 6
		memory[30] = 0b01100000;
		memory[31] = 0b10000000;
		memory[32] = 0b11100000;
		memory[33] = 0b10010000;
		memory[34] = 0b01100000;
		// 7   
		memory[35] = 0b11110000;
		memory[36] = 0b00010000;
		memory[37] = 0b00100000;
		memory[38] = 0b01000000;
		memory[39] = 0b01000000;
		// 8
		memory[40] = 0b01100000;
		memory[41] = 0b10010000;
		memory[42] = 0b01100000;
		memory[43] = 0b10010000;
		memory[44] = 0b01100000;
		// 9   
		memory[45] = 0b01100000;
		memory[46] = 0b10010000;
		memory[47] = 0b01110000;
		memory[48] = 0b00010000;
		memory[49] = 0b01100000;
		// A
		memory[50] = 0b01100000;
		memory[51] = 0b10010000;
		memory[52] = 0b11110000;
		memory[53] = 0b10010000;
		memory[54] = 0b10010000;
		// B   
		memory[55] = 0b11100000;
		memory[56] = 0b10010000;
		memory[57] = 0b11100000;
		memory[58] = 0b10010000;
		memory[59] = 0b11100000;
		// C
		memory[60] = 0b01100000;
		memory[61] = 0b10000000;
		memory[62] = 0b10000000;
		memory[63] = 0b10000000;
		memory[64] = 0b01100000;
		// D   
		memory[65] = 0b11100000;
		memory[66] = 0b10010000;
		memory[67] = 0b10010000;
		memory[68] = 0b10010000;
		memory[69] = 0b11100000;
		// E
		memory[70] = 0b11110000;
		memory[71] = 0b10000000;
		memory[72] = 0b11100000;
		memory[73] = 0b10000000;
		memory[74] = 0b11110000;
		// F   
		memory[75] = 0b11110000;
		memory[76] = 0b10000000;
		memory[77] = 0b11100000;
		memory[78] = 0b10000000;
		memory[79] = 0b10000000;

		// load game into memory
		basic_ifstream<BYTE> fileIn = basic_ifstream<BYTE>(game, ios::binary | ios::ate);
		if (!fileIn.is_open()) {
			exit(1);
		}
		int length = fileIn.tellg();
		fileIn.clear();
		fileIn.seekg(0, ios::beg);
		fileIn.read(&(memory[0x200]), length);
		fileIn.close();

	}

	void clockCPU() {
		//fetch, decode and execute the instruction
		decodeOpcode();

		//decrement timers
		if (delayTimer != 0) delayTimer--;
		if (soundTimer != 0) sound::playSound(500, soundTimer * 16.66, 2000, 1); soundTimer = 0;
	}
};

int main(int argc, char* argv[]) {
	FreeConsole();
	sf::RenderWindow window(sf::VideoMode(screenWidth * scale, screenHeight * scale), "CHIP8 Emulator");
	window.setFramerateLimit(250);
	initializeScreen();
	srand(time(NULL));
	handleArguments(argc, argv);

	chip8 emu;
	emu.resetCPU();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed: window.close(); break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
					window.close(); break;
				}
				break;
			}
		}

		emu.clockCPU();

		window.clear();
		for (int i = 0; i < screenWidth * screenHeight; i++) {
			window.draw(screen[i]);
		}
		window.display();


	}

	return 0;
}