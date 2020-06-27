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

#pragma once

#include "Application/PickableApplication.h"
#include "DrawableObjects/PickableObject.h"

#include <Corrade/Containers/Array.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/Phong.h>

/****************************************************************************************************/
class MyApplication final : public PickableApplication  {
public:
    explicit MyApplication(const Arguments& arguments);

protected:
    void drawEvent() override;

    /* Draw pickable spheres */
    Containers::Array<Vector3>         m_Points;
    Containers::Array<PickableObject*> m_DrawablePoints;
    Shaders::Phong                     m_SphereShader { Shaders::Phong::Flag::ObjectId };
    GL::Mesh m_MeshSphere{ NoCreate };
};

/****************************************************************************************************/
MAGNUM_APPLICATION_MAIN(MyApplication)
