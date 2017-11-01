#include "LTexture.h"
#include <string>
#include <regex>
#include <Windows.h>

SDL_Window* gWindow = NULL;

int const SCREEN_WIDTH = 256;
int const SCREEN_HEIGHT = 512;

SDL_Rect gSpriteClips[20];
LTexture gSpriteSheetTexture;

char grid[5][4] = {
	'd', 'c' , 'b', '/', // d = CE, c = C, b = backspace
	'7', '8' , '9', '*',
	'4', '5' , '6', '-',
	'1', '2' , '3', '+',	
	's', '0' , '.', '=' // s = sign
};

void drawSignNum(char n, float x0, float y0);

bool init();
bool loadMedia();
void close();

int wmain() {

	FreeConsole();

	if (!init()) {
		std::cout << "Window could not be initialized! \n";
	}
	else {
		if (!loadMedia()) {
			std::cout << "Media could not be loaded! \n";
		}
		else {
			/*SDL_Surface* icoSurf = IMG_Load("calc.ico");
			if (icoSurf == NULL) std::cout << "Errr" << std::endl;
			SDL_SetWindowIcon(gWindow, icoSurf);*/
			bool quit = false;
			SDL_Event e;
			std::string screen = "0";
			std::string screen2 = "";
			double result = 0;
			bool newLine = false;
			char sign = 0;
			bool eqAllowed = false;
			bool endEq = false;
			bool endEq2 = false;
			int line = 0;
			std::string re;
			std::string regex = "^[0]*?\.?[0]*?$";
			std::string input;
			while (!quit) {
				SDL_Rect hoverRect;

				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{

					}
					else if (e.type == SDL_MOUSEMOTION) {
						int x = 64 * (e.motion.x / 64);
						int y = 64 * (e.motion.y / 64);
						if (e.motion.x < 1 || e.motion.x > 254 || e.motion.y > 510) {
							x = -64;
							y = -64;
						}
						if (y < 192) y = -64;
						hoverRect.x = x;
						hoverRect.y = y;
						hoverRect.w = 64;
						hoverRect.h = 64;
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN) {
						if (e.button.button == SDL_BUTTON_LEFT) {
							int x1 = e.button.x / 64;
							int y1 = e.button.y / 64 - 3;
							if (x1 >= 0 && x1 < 4 && y1 >= 0 && y1 < 5) {
								std::cout << grid[y1][x1] << std::endl;
								if (grid[y1][x1] == '=' && endEq2 == true) {
									screen = re;
									newLine = true;

									/*int offset = 1;
									if (screen2.find_last_not_of('0') == screen2.find('.')) {
										offset = 0;
									}
									screen2.erase(screen2.find_last_not_of('0') + offset, std::string::npos);*/

									//regex
									//if (!std::regex_match(screen, std::regex(regex))) break;

									if (std::regex_match(screen2, std::regex("^[0]*?\\.?[0]*?$"))) {
										std::cout << screen << " Ok" << std::endl;
										screen2 = "0";
									}

									switch (sign)
									{
									case '/': result = result / std::stod(screen2);
										break;
									case '*': result = result * std::stod(screen2);
										break;
									case '+': result = result + std::stod(screen2);
										break;
									case '-': result = result - std::stod(screen2);
										break;
									default:
										break;
									}
								}
								else if (grid[y1][x1] == '=' && eqAllowed) {

									/*int offset = 1;
									if (screen2.find_last_not_of('0') == screen2.find('.')) {
										offset = 0;
									}
									screen2.erase(screen2.find_last_not_of('0') + offset, std::string::npos);*/

									//regex
									//if (!std::regex_match(screen2, std::regex(regex))) break;

									if (std::regex_match(screen2, std::regex("^[0]*?\\.?[0]*?$"))) {
										std::cout << screen << " Ok" << std::endl;
										screen2 = "0";
									}


									switch (sign)
									{
									case '/': result = result / std::stod(screen2);
										break;
									case '*': result = result * std::stod(screen2);
										break;
									case '+': result = result + std::stod(screen2);
										break;
									case '-': result = result - std::stod(screen2);
										break;
									default:
										break;
									}

									if (screen2[screen2.length() - 1] == '.') {
										screen2.replace(screen2.length() - 1, 1, "");
									}

									endEq = true;
									endEq2 = true;
								}
								else if (grid[y1][x1] == 's') {
									if (sign == 0) {
										if (!std::regex_match(screen, std::regex("^[0]*?\\.?[0]*?$"))) {
											if (screen[0] == '-') {
												screen.replace(0, 1, "");
											}
											else if (screen == "") {
												screen = "-";
											}
											else {
												screen = "-" + screen;
												std::cout << screen << std::endl;
											}
										}
									}
									else if (line == 2 && endEq == false) {
										if (!std::regex_match(screen2, std::regex("^[0]*?\\.?[0]*?$"))) {
											if (screen2[0] == '-') {
												screen2.replace(0, 1, "");
												std::cout << screen2 << std::endl;
											}
											else if (screen2 == "") {
												screen2 = "-";
												std::cout << screen2 << std::endl;
											}
											else {
												screen2 = "-" + screen2;
												std::cout << screen2 << std::endl;
											}
										}
									}

								}
								else if (grid[y1][x1] == '/' ||
									grid[y1][x1] == '*' ||
									grid[y1][x1] == '-' ||
									grid[y1][x1] == '+') {
									if (endEq == false) {
										if (screen != "" && screen != "-" && screen != "-." && screen != ".") {
											if (newLine == false) {
												sign = grid[y1][x1];
												line = 2;

												/*int offset = 1;
												if (screen.find_last_not_of('0') == screen.find('.')) {
													offset = 0;
												}
												screen.erase(screen.find_last_not_of('0') + offset, std::string::npos);*/

												// THE FIRST PROBLEM IS HERE
												if (!std::regex_match(screen, std::regex("^[0]*?\\.?[0]*?$"))) {
													std::cout << screen << std::endl;
													result = std::stod(screen);
												}
												else {
													std::cout << screen << std::endl;
													result = 0;
													screen = "0";
												}

												if (screen[screen.length() - 1] == '.') {
													screen.replace(screen.length() - 1, 1, "");
												}

											}
											newLine = true;
										}
									}
									else {
										screen = re;
										screen2 = "";
										sign = grid[y1][x1];
										line = 2;
										newLine = true;
										endEq = false;
										endEq2 = false;
										eqAllowed = false;
									}
								}
								else if (grid[y1][x1] == 'd') {
									if (endEq == false) {
										if (sign != 0 && screen != "" && screen2 == "" && newLine == true) {
											sign = 0;
											newLine = false;
											screen2 == "";
										}
										else if (newLine == true) {
											screen2 = "";
										}
										else {
											screen = "";
											result = 0;
										}
									}
								}
								else if (grid[y1][x1] == 'c') {
									line = 0;
									screen = "";
									screen2 = "";
									result = 0;
									newLine = false;
									sign = 0;
									eqAllowed = false;
									endEq = false;
									endEq2 = false;
								}
								else if (grid[y1][x1] == 'b') {
									if (newLine == false && screen != "" && endEq == false) {
										screen.pop_back();
										result = 0;
										if (screen != "") {
											result += std::stod(screen);
										}
									}
									else if (sign != 0 && screen != "" && screen2 == "" && newLine == true && endEq == false) {
										sign = 0;
										newLine = false;
										result = std::stod(screen);
										screen2 == "";
									}
									else if (sign != 0 && screen2 != "" && screen != "" && newLine == true && endEq == false) {
										screen2.pop_back();
									}
								}
								else {
									if (newLine == true) {
										if (grid[y1][x1] != '=' && endEq == false && screen2.length() <= 12) {
											//if (screen2 == "0") screen2 = "";
											line = 2;
											if (char(grid[y1][x1] == '.')) {
												std::size_t found = screen2.find('.');
												if (found == std::string::npos) {

													screen2 += char(grid[y1][x1]);

												}
											}
											else {
												screen2 += char(grid[y1][x1]);

											}
										}

										/*if (screen2[0] == '0' && !std::regex_match(screen2, std::regex("^[0]*?\\.?[0]*?$"))) {
											screen2.replace(0, 1, "");
										}*/

										if (screen2[0] == '0' && screen2.length() > 1) {
											screen2.replace(0, 1, "");
										}

										if (screen2[0] == '.') {
											screen2 = "0" + screen2;
										}
									}
									else {
										if (grid[y1][x1] != '=' && grid[y1][x1] != 's' && endEq == false && screen.length() <= 12) {
											//if (screen == "0") screen = "";
											line = 0;
											if (char(grid[y1][x1] == '.')) {
												std::size_t found = screen.find('.');
												if (found == std::string::npos) {

													screen += char(grid[y1][x1]);

												}
											}
											else {
												screen += char(grid[y1][x1]);

											}


											if (screen[0] == '0' && screen.length() > 1) {
												screen.replace(0, 1, "");
											}


											if (screen[0] == '.') {
												screen = "0" + screen;
											}
										}
									}



									/*if (screen != "" && screen2 != "") {
										eqAllowed = true;
									}

									if (!(screen == "0" || screen2 == "0")) {
										if (!std::regex_match(screen2, std::regex(regex)) || !std::regex_match(screen, std::regex(regex)))
										{
											eqAllowed = false;
										}
									}

									if (sign == '/'
										&& screen2 == "0") eqAllowed = false;*/



									if (screen != "" && screen2 != "") {
										eqAllowed = true;

										if (sign == '/' && std::regex_match(screen2, std::regex("^[0]*?\\.?[0]*?$"))) eqAllowed = false;
									} // std::stod(screen2) == 0
									else eqAllowed = false;

								}
							}

							if (screen == "" && screen2 == "" && sign == 0 && newLine == false) {
								screen = "0";
							}
						}
					}
				}



				SDL_SetRenderDrawColor(LTexture::gRenderer, 0, 0, 0, 0);
				SDL_RenderClear(LTexture::gRenderer);

				//highlighting signs and numbers
				SDL_SetRenderDrawColor(LTexture::gRenderer, 105, 105, 105, 0);
				SDL_RenderFillRect(LTexture::gRenderer, &hoverRect);




				//display
				SDL_SetRenderDrawColor(LTexture::gRenderer, 40, 40, 40, 0);
				SDL_Rect displayRect;
				displayRect.x = 0;
				displayRect.y = 0;
				displayRect.w = 256;
				displayRect.h = 192;
				SDL_RenderFillRect(LTexture::gRenderer, &displayRect);

				//screen + alg
				int x2 = SCREEN_WIDTH - 16;
				for (int i = screen.length(); i >= 0; i--)
				{
					drawSignNum(screen[i], x2, 8);
					//std::cout << screen << std::endl;
					x2 -= 18;
				}

				if (newLine == true)
					drawSignNum(sign, SCREEN_WIDTH - 32, 40);

				x2 = SCREEN_WIDTH - 16;
				for (int i = screen2.length(); i >= 0; i--)
				{
					drawSignNum(screen2[i], x2, 64);
					//std::cout << screen << std::endl;
					x2 -= 18;
				}

				if (endEq) {
					x2 = SCREEN_WIDTH - 16;
					drawSignNum('=', SCREEN_WIDTH - 32, 96);

					int offset = 1;
					re = std::to_string(result);
					if (re.find_last_not_of('0') == re.find('.')) {
						offset = 0;
					}

					re.erase(re.find_last_not_of('0') + offset, std::string::npos);

					if (std::regex_match(re, std::regex("^-[0]*?\\.?[0]*?$"))) {
						re = "0";
					}

					for (int i = re.length(); i >= 0; i--)
					{
						drawSignNum(re[i], x2, 120);
						x2 -= 18;
					}
				}
				//lines

				//vertical
				for (int i = 0; i < SCREEN_WIDTH; i += 64)
				{
					SDL_RenderDrawLine(LTexture::gRenderer, i, 192, i, SCREEN_HEIGHT);
				}

				//horizontal
				for (int i = 256; i < SCREEN_HEIGHT; i += 64)
				{
					SDL_RenderDrawLine(LTexture::gRenderer, 0, i, SCREEN_WIDTH, i);
				}

				//draw
				SDL_SetRenderDrawColor(LTexture::gRenderer, 0, 0, 0, 0);
				int x = 0;
				int y = SCREEN_HEIGHT - 320;
				int z = 0;
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						gSpriteSheetTexture.render(x, y, &gSpriteClips[z], 2, 2);
						z++;
						x += 64;
					}
					x = 0;
					y += 64;
				}



				SDL_RenderPresent(LTexture::gRenderer);
			}
		}
	}

	//system("pause");
	close();
	return 0;
}

void drawSignNum(char n, float x0, float y0)
{
	switch (n)
	{
	case '/':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[3]);
		break;
	case '7':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[4]);
		break;
	case '8':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[5]);
		break;
	case '9':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[6]);
		break;
	case '*':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[7]);
		break;
	case '4':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[8]);
		break;
	case '5':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[9]);
		break;
	case '6':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[10]);
		break;
	case '-':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[11]);
		break;
	case '1':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[12]);
		break;
	case '2':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[13]);
		break;
	case '3':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[14]);
		break;
	case '+':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[15]);
		break;
	case '0':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[17]);
		break;
	case '.':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[18]);
		break;
	case '=':
		gSpriteSheetTexture.render(x0, y0, &gSpriteClips[19]);
		break;
	default:
		break;
	}
}

bool init() {
	bool success = true;

	gWindow = SDL_CreateWindow("Calculator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		std::cout << "Window could not be created! SDL Error: \n" << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		LTexture::gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (LTexture::gRenderer == NULL) {
			std::cout << "Renderer could not be created! SDL Error: \n" << SDL_GetError() << std::endl;
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(LTexture::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				std::cout << "SDL_image could not initialize! SDL_image Error: \n" << IMG_GetError() << std::endl;
				success = false;
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;
	if (!gSpriteSheetTexture.loadFromFile("sprite1.png")) {
		std::cout << "Failed to load texture image!\n";
		success = false;
	}
	else {

		int y = -1;
		int x = 0;
		for (int i = 0; i < 20; i++)
		{
			if (x > 96) x = 0;
			if (i % 4 == 0 && i != 0) y += 32;
			gSpriteClips[i].x = x;
			gSpriteClips[i].y = y;
			gSpriteClips[i].w = 32;
			gSpriteClips[i].h = 32;
			x += 32;
		}
	}

	return success;
}

void close() {
	gSpriteSheetTexture.free();

	SDL_DestroyRenderer(LTexture::gRenderer);
	SDL_DestroyWindow(gWindow);
	LTexture::gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}