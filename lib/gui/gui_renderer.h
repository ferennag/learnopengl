#pragma once

class GuiRenderer {
public:
  virtual ~GuiRenderer() = default;

  virtual void Render() = 0;
};
