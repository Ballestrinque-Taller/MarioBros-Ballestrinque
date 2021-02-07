#include "TextWriter.h"
#include "Log.h"


TextWriter::TextWriter(){
    TTF_Init();
    fuente = TTF_OpenFont("./res/font.ttf", 20);
}

void TextWriter::set_msg_rect(int x, int y, int h, int w){
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;
}

void TextWriter::write_text(const char* texto, SDL_Renderer* renderer){
    SDL_Color white = {255, 255, 255};
    surfaceMessage = TTF_RenderText_Solid( fuente, texto, white);
    mensaje = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_RenderCopy(renderer, mensaje, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(mensaje);
}

TextWriter::~TextWriter(){
    //LOG(Log::DEBUG)<<"Destruyendo texto en pantalla..."<<std::endl;
    TTF_Quit();
}