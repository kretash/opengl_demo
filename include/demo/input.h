#pragma once

#include <memory>
#include <map>
#include "SDL/SDL.h"

struct mouse {
  float x = 0.0f, y = 0.0f;
  bool lmb = false, mmb = false, rmb = false;
};

class Input {
public:

  static std::shared_ptr<Input> get_instace() {
    static std::shared_ptr<Input> instance = std::make_shared<Input>();
    return instance;
  }

  ~Input() {};

  void update() {
    SDL_Event event_;
    while( SDL_PollEvent( &event_ ) ) {
      switch( event_.type ) {
      case SDL_QUIT:
        m_quit = true;
        break;
      case SDL_KEYDOWN:
        if( m_downkeys.find( event_.key.keysym.sym ) == m_downkeys.end() )
          m_downkeys.insert( std::make_pair( event_.key.keysym.sym, true ) );
        else
          m_downkeys[event_.key.keysym.sym] = true;
        break;
      case SDL_KEYUP:
        m_downkeys[event_.key.keysym.sym] = false;
        break;
      case SDL_MOUSEBUTTONDOWN:
        m_mouse.lmb |= event_.button.button == SDL_BUTTON_LEFT;
        m_mouse.mmb |= event_.button.button == SDL_BUTTON_MIDDLE;
        m_mouse.rmb |= event_.button.button == SDL_BUTTON_RIGHT;
        break;
      case SDL_MOUSEBUTTONUP:
        m_mouse.lmb &= !( event_.button.button == SDL_BUTTON_LEFT );
        m_mouse.mmb &= !( event_.button.button == SDL_BUTTON_MIDDLE );
        m_mouse.rmb &= !( event_.button.button == SDL_BUTTON_RIGHT );
        break;
      case SDL_MOUSEMOTION:
        m_mouse.x = event_.button.x;
        m_mouse.y = event_.button.y;
      default:
        break;
      }
    }
  }

  bool key_down( SDL_Keycode key ) {
    auto found = m_downkeys.find( key );
    if( found == m_downkeys.end() ) return false;
    return m_downkeys[key];
  }

  mouse get_mouse() { return m_mouse; }
  bool quit() { return m_quit; }

  Input() {};
private:
  std::map<SDL_Keycode, bool> m_downkeys;
  bool m_quit = false;
  mouse m_mouse;
};