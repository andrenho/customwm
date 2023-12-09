#ifndef CUSTOMWM_FILLPROGRAM_HH
#define CUSTOMWM_FILLPROGRAM_HH

#include "program.hh"
#include "theme/types/types.hh"

class FillProgram : public Program {
public:
    void upload() override;
    void draw(Size const& viewport_sz, Color const& color, Rectangle const& rect) const;

protected:
    const char *name() const override { return "fill"; }
    const char *vertex_shader() const override;
    const char *fragment_shader() const override;

private:
    GLint projection_loc;
    GLint bg_color_loc;
};

#endif //CUSTOMWM_FILLPROGRAM_HH
