/**
 * Copyright 2020 Nghia Truong <nghiatruong.vn@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DrawableObjects/PickableObject.h"
#include "MyApplication.h"

#include <Corrade/Containers/GrowableArray.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Primitives/Icosphere.h>

using namespace Magnum::Math::Literals;

/****************************************************************************************************/
MyApplication::MyApplication(const Arguments& arguments) :
    PickableApplication{"Graphics Template Application ", arguments} {
    m_MeshSphere = MeshTools::compile(Primitives::icosphereSolid(3));

    for(size_t i = 0; i < 8; ++i) {
        const Vector3 tmpPos = Vector3(std::rand(), std::rand(), std::rand()) /
                               Float(RAND_MAX);
        arrayAppend(m_Points,         Containers::InPlaceInit, tmpPos * 2.0f - Vector3{ 1.0f });

        arrayAppend(m_DrawablePoints, Containers::InPlaceInit,
                    new PickableObject{ m_SphereShader,
                                        Color3{ tmpPos },
                                        m_MeshSphere,
                                        &m_Scene,
                                        &m_Drawables });

        m_Points[i].y() += 1.0f;
        m_DrawablePoints[i]->setTransformation(Matrix4::translation(m_Points[i]) *
                                               Matrix4::scaling(Vector3(0.05f)));
        m_mDrawableIdxToPointIdx[m_DrawablePoints[i]->idx()] = i;
    }
}

/****************************************************************************************************/
void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
    ImGuiApplication::beginFrame();

    /* Update camera */
    m_Camera->update();

    /* Draw to custom framebuffer */
    m_FrameBuffer
        .clearColor(0, m_BkgColor)
        .clearColor(1, Vector4ui{})
        .clearDepth(1.0f)
        .bind();

    /* Draw other objects (grid) */
    m_Camera->draw(m_Drawables);

    /* Bind the default framebuffer back, as only the clickable objects need to be render to custom framebuffer */
    GL::defaultFramebuffer.bind();

    /* Blit color to window framebuffer */
    m_FrameBuffer.mapForRead(GL::Framebuffer::ColorAttachment{ 0 });
    GL::AbstractFramebuffer::blit(m_FrameBuffer, GL::defaultFramebuffer,
                                  { {}, m_FrameBuffer.viewport().size() }, GL::FramebufferBlit::Color);

    /* Menu for controllers */
    if(m_bShowMenu) {
        showMenuHeader();
        showMenuFooter();
    }

    /* Manipulate nodes' transformation */
    PickableObject* selectedPoint = PickableObject::selectedObj();
    if(selectedPoint) {
        if(selectedPoint->isSelectable()
           && selectedPoint->isMovable()) {
            ImGui::Begin("Editor");
            std::string str = "Point: #" + std::to_string(selectedPoint->idx());
            ImGui::Text("%s", str.c_str());
            ImGui::Separator();
            ImGui::Spacing();

            Matrix4 objMat = selectedPoint->transformation();
            if(editPointTransformation(objMat)) {
                selectedPoint->setTransformation(objMat);                       /* Update drawable transformation */
                setPointTransformation(selectedPoint->idx(), objMat, m_Points); /* Update real data point */
            }
            ImGui::End();
        }
    }

    ImGuiApplication::endFrame();
    swapBuffers();
    redraw();
}
