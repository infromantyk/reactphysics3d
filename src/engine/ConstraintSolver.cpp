/********************************************************************************
* ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/      *
* Copyright (c) 2010-2013 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

// Libraries
#include "ConstraintSolver.h"
#include "Profiler.h"

using namespace reactphysics3d;

// Constructor
ConstraintSolver::ConstraintSolver(std::set<Constraint*>& joints,
                                   std::vector<Vector3>& constrainedLinearVelocities,
                                   std::vector<Vector3>& constrainedAngularVelocities,
                                   const std::map<RigidBody*, uint>& mapBodyToVelocityIndex)
                 : mJoints(joints), mConstrainedLinearVelocities(constrainedLinearVelocities),
                   mConstrainedAngularVelocities(constrainedAngularVelocities),
                   mMapBodyToConstrainedVelocityIndex(mapBodyToVelocityIndex) {

}

// Destructor
ConstraintSolver::~ConstraintSolver() {

}

// Initialize the constraint solver
void ConstraintSolver::initialize(decimal dt) {

    PROFILE("ConstraintSolver::initialize()");

    // Set the current time step
    mTimeStep = dt;
}

// Solve the constraints
void ConstraintSolver::solve() {

    PROFILE("ConstraintSolver::solve()");

}
