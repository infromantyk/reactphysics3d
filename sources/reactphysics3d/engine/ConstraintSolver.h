/***************************************************************************
* Copyright (C) 2009      Daniel Chappuis                                  *
****************************************************************************
* This file is part of ReactPhysics3D.                                     *
*                                                                          *
* ReactPhysics3D is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU Lesser General Public License as published *
* by the Free Software Foundation, either version 3 of the License, or     *
* (at your option) any later version.                                      *
*                                                                          *
* ReactPhysics3D is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU Lesser General Public License for more details.                      *
*                                                                          *
* You should have received a copy of the GNU Lesser General Public License *
* along with ReactPhysics3D. If not, see <http://www.gnu.org/licenses/>.   *
***************************************************************************/

#ifndef CONSTRAINTSOLVER_H
#define CONSTRAINTSOLVER_H

// Libraries
#include "../typeDefinitions.h"
#include "../constraint/Constraint.h"
#include "../mathematics/lcp/LCPSolver.h"
#include "ContactCache.h"
#include "PhysicsWorld.h"
#include <map>
#include <set>

// ReactPhysics3D namespace
namespace reactphysics3d {

// Constants
const uint MAX_LCP_ITERATIONS = 10;     // Maximum number of iterations when solving a LCP problem

 /*  -------------------------------------------------------------------
    Class ConstrainSolver :
        This class represents the constraint solver. The goal is to
        solve A constraint-base LCP problem.
    -------------------------------------------------------------------
*/
class ConstraintSolver {
    protected:
        PhysicsWorld* physicsWorld;                             // Reference to the physics world
        LCPSolver* lcpSolver;                                   // LCP Solver
        ContactCache contactCache;                              // Contact cache
        std::vector<Constraint*> activeConstraints;             // Current active constraints in the physics world
        uint nbConstraints;                                     // Total number of constraints (with the auxiliary constraints)
        std::set<Body*> constraintBodies;                       // Bodies that are implied in some constraint
        uint nbBodies;                                          // Current number of bodies in the physics world
        std::map<Body*, uint> bodyNumberMapping;                // Map a body pointer with its index number
        Body*** bodyMapping;                                    // 2-dimensional array that contains the mapping of body reference
                                                                // in the J_sp and B_sp matrices. For instance the cell bodyMapping[i][j] contains
                                                                // the pointer to the body that correspond to the 1x6 J_ij matrix in the
                                                                // J_sp matrix. A integer body index refers to its index in the "bodies" std::vector
        Matrix** J_sp;                                          // 2-dimensional array thar correspond to the sparse representation of the jacobian matrix of all constraints
                                                                // The dimension of this array is nbConstraints times 2. Each cell will contain
                                                                // a 1x6 matrix
        Matrix** B_sp;                                          // 2-dimensional array that correspond to a useful matrix in sparse representation
                                                                // The dimension of this array is 2 times nbConstraints. Each cell will contain
                                                                // a 6x1 matrix
        Vector b;                                               // Vector "b" of the LCP problem
        Vector lambda;                                          // Lambda vector of the LCP problem
        Vector lambdaInit;                                      // Lambda init vector for the LCP solver
        Vector errorValues;                                     // Error vector of all constraints
        Vector lowerBounds;                                     // Vector that contains the low limits for the variables of the LCP problem
        Vector upperBounds;                                     // Vector that contains the high limits for the variables of the LCP problem
        Matrix* Minv_sp;                                        // Sparse representation of the Matrix that contains information about mass and inertia of each body
                                                                // This is an array of size nbBodies that contains in each cell a 6x6 matrix
        Vector* V1;                                             // Array that contains for each body the Vector that contains linear and angular velocities
                                                                // Each cell contains a 6x1 vector with linear and angular velocities
        Vector* Vconstraint;                                    // Same kind of vector as V1 but contains the final constraint velocities
        Vector* Fext;                                           // Array that contains for each body the vector that contains external forces and torques
                                                                // Each cell contains a 6x1 vector with external force and torque.
        void allocate();                                        // Allocate all the matrices needed to solve the LCP problem
        void fillInMatrices();                                  // Fill in all the matrices needed to solve the LCP problem
        void computeVectorB(double dt);                         // Compute the vector b
        void computeMatrixB_sp();                               // Compute the matrix B_sp
        void computeVectorVconstraint(double dt);               // Compute the vector V2
        void updateContactCache();                              // Clear and Fill in the contact cache with the new lambda values

    public:
        ConstraintSolver(PhysicsWorld* world);                                  // Constructor
        virtual ~ConstraintSolver();                                            // Destructor
        void solve(double dt);                                                  // Solve the current LCP problem
        bool isConstrainedBody(Body* body) const;                               // Return true if the body is in at least one constraint
        Vector3D getConstrainedLinearVelocityOfBody(Body* body);                // Return the constrained linear velocity of a body after solving the LCP problem
        Vector3D getConstrainedAngularVelocityOfBody(Body* body);               // Return the constrained angular velocity of a body after solving the LCP problem
        void freeMemory();                                                      // Free the memory that was allocated in the allocate() method
};

// Return true if the body is in at least one constraint
inline bool ConstraintSolver::isConstrainedBody(Body* body) const {
    if(constraintBodies.find(body) != constraintBodies.end()) {
        return true;
    }
    return false;
}

// Return the constrained linear velocity of a body after solving the LCP problem
inline Vector3D ConstraintSolver::getConstrainedLinearVelocityOfBody(Body* body) {
    assert(isConstrainedBody(body));
    Vector vec = Vconstraint[bodyNumberMapping[body]].getSubVector(0, 3);
    return Vector3D(vec.getValue(0), vec.getValue(1), vec.getValue(2));

}

// Return the constrained angular velocity of a body after solving the LCP problem
inline Vector3D ConstraintSolver::getConstrainedAngularVelocityOfBody(Body* body) {
    assert(isConstrainedBody(body));
    Vector vec = Vconstraint[bodyNumberMapping[body]].getSubVector(3, 3);
    return Vector3D(vec.getValue(0), vec.getValue(1), vec.getValue(2));
}


} // End of ReactPhysics3D namespace

#endif