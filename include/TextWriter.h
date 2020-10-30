#ifndef MARIOBROS_BALLESTRINQUE_TEXTWRITER_H
#define MARIOBROS_BALLESTRINQUE_TEXTWRITER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


class TextWriter {
    public:
        ~TextWriter();
        TextWriter();
        void write_text(const char* texto, SDL_Renderer* renderer);
        void set_msg_rect(int x, int y, int h, int w);

    protected:
        SDL_Texture* mensaje;
        SDL_Surface* surfaceMessage;

    private:
        TTF_Font* fuente;
        SDL_Rect Message_rect;
};


#endif //MARIOBROS_BALLESTRINQUE_TEXTWRITER_H
