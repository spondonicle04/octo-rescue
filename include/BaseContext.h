// BaseContext.h

#ifndef BASECONTEXT_H
#define BASECONTEXT_H

class BaseContext {
public:
  virtual void onEnter() {}         // Called when context becomes active
  virtual void onExit() {}          // Called when context is left
  virtual void update() = 0;        // Called every loop
  virtual void draw() = 0;          // Called every frame/tick
  virtual void handleInput(int btn) {} // Button or encoder input handler
};

#endif
