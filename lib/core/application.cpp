#include "application.h"

void Application::HandleResize(int width, int height) {
  mWidth = width;
  mHeight = height;
}

float Application::GetAspectRatio() const {
  return static_cast<float>(mWidth) / static_cast<float>(mHeight);
}

bool Application::Load(int width, int height) {
  mWidth = width;
  mHeight = height;
  return true;
}
