#include "stdafx.h"
#include "TerrainGenTextures.h"
#include "Errors.h"

#include <Vorb/graphics/SamplerState.h>
#include <Vorb/graphics/GpuMemory.h>

TerrainGenTextures::~TerrainGenTextures() {
    destroy();
}

void TerrainGenTextures::init(const ui32v2& dims) {
    m_dims = dims;

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create texture targets
    glGenTextures(1, m_textures);
    initTarget(m_dims, m_tex.height_temp_hum, TERRAINGEN_INTERNAL_FORMAT, 0);
   
    // Set the output location for pixels
    VGEnum bufs[1] = {
        GL_COLOR_ATTACHMENT0
    };
    glDrawBuffers(1, bufs);

    // Unbind used resources
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO: Change The Memory Usage Of The GPU
}

void TerrainGenTextures::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_dims.x, m_dims.y);
}

void TerrainGenTextures::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TerrainGenTextures::destroy() {
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
    if (m_tex.height_temp_hum != 0) {
        glDeleteTextures(1, m_textures);
        m_tex = { 0 };
    }
}

void TerrainGenTextures::initTarget(const ui32v2& size, const ui32& texID, const vg::TextureInternalFormat& format, const ui32& attachment) {
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, (VGEnum)format, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    vg::SamplerState::POINT_CLAMP.set(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, texID, 0);
}