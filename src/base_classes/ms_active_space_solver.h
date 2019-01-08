/*
 * @BEGIN LICENSE
 *
 * Forte: an open-source plugin to Psi4 (https://github.com/psi4/psi4)
 * that implements a variety of quantum chemistry methods for strongly
 * correlated electrons.
 *
 * Copyright (c) 2012-2019 by its authors (see COPYING, COPYING.LESSER, AUTHORS).
 *
 * The copyrights for code used from other parties are included in
 * the corresponding files.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 * @END LICENSE
 */

#ifndef _ms_active_space_solver_h_
#define _ms_active_space_solver_h_

#include <vector>
#include <string>

#include "base_classes/state_info.h"

namespace forte {

class ActiveSpaceSolver;
class ActiveSpaceIntegrals;
class ForteIntegrals;
class ForteOptions;
class MOSpaceInfo;
class Reference;
class SCFInfo;

/**
 * @class MSActiveSpaceSolver
 *
 * @brief General class for a multi-state active space solver
 *
 * This class is the base class for methods that solve for the wavefunction in a
 * small subset of the full orbital space (<30-40 orbitals).
 * This class is responsible for creating and storing the integrals used by
 * active space solvers, which are held by an ActiveSpaceIntegrals object.
 *
 * @note By default, this class assumes that the active orbitals are stored in the MOSpaceInfo
 * object in the space labeled "ACTIVE". Orbitals in the space "RESTRICTED_DOCC"
 * are not correlated and are trated via effective scalar and one-body interactions.
 */
class MSActiveSpaceSolver {
  public:
    // ==> Class Constructor and Destructor <==
    /**
     * @brief ActiveSpaceSolver Constructor for a multi-state computation
     * @param states_weights A list of electronic states and their weights stored as vector of
     *        pairs [(state_1, [w_11, w_12, ..., w_1m]), (state_2, [w_21, w_22, ..., w_n]), ...]
     *        where:
     *            state_i specifies the symmetry of a state
     *            w_ij is the weight of the j-th state of symmetry state_i
     * @param state information about the electronic state
     * @param mo_space_info a MOSpaceInfo object
     * @param as_ints integrals for active space
     */
    MSActiveSpaceSolver(const std::string& type,
                        std::vector<std::pair<StateInfo, std::vector<double>>>& state_weights_list,
                        std::shared_ptr<SCFInfo> scf_info,
                        std::shared_ptr<MOSpaceInfo> mo_space_info,
                        std::shared_ptr<ActiveSpaceIntegrals> as_ints,
                        std::shared_ptr<ForteOptions> options);

    //    // ==> Class Interface <==

    /// Compute the energy and return it
    double compute_energy();

    void print_options();

    //    /// Returns the reference
    //    Reference get_reference() = 0;

    //    /// Set options from an option object
    //    /// @param options the options passed in
    //    void set_options(std::shared_ptr<ForteOptions> options) = 0;

    //    // ==> Base Class Functionality (inherited by derived classes) <==

    //    /// Pass a set of ActiveSpaceIntegrals to the solver (e.g. an effective Hamiltonian)
    //    /// @param as_ints the integrals passed in
    //    void set_active_space_integrals(std::shared_ptr<ActiveSpaceIntegrals> as_ints);

    //    /// Return the eigenvalues
    //    psi::SharedVector evals();

    //    // ==> Base Class Handles Set Functions <==

    //    /// Set the energy convergence criterion
    //    /// @param value the convergence criterion in a.u.
    //    void set_e_convergence(double value);

    //    /// Set the number of desired roots
    //    /// @param value the number of desired roots
    //    void set_nroot(int value);

    //    /// Set the root that will be used to compute the properties
    //    /// @param the root (root = 0, 1, 2, ...)
    //    void set_root(int value);

    //    /// Set the maximum RDM computed (0 - 3)
    //    /// @param value the rank of the RDM
    //    void set_max_rdm_level(int value);

    //    /// Set the print level
    //    /// @param level the print level (0 = no printing, 1 default)
    //    void set_print(int level);

  protected:
    std::string type_;
    //    /// The list of active orbitals (absolute ordering)
    //    std::vector<size_t> active_mo_;

    //    /// The list of doubly occupied orbitals (absolute ordering)
    //    std::vector<size_t> core_mo_;

    /// A list of electronic states and their weights stored as vector of pairs
    ///   [(state_1, [w_11, w_12, ..., w_1m]), (state_2, [w_21, w_22, ..., w_n]), ...]
    /// where state_i specifies the symmetry of a state and w_ij are are list of weights
    /// for the states of the same symmetry
    std::vector<std::pair<StateInfo, std::vector<double>>>& state_weights_list_;

    //    StateInfo state_;

    std::shared_ptr<SCFInfo> scf_info_;
    /// The MOSpaceInfo object
    std::shared_ptr<MOSpaceInfo> mo_space_info_;

    std::shared_ptr<ActiveSpaceIntegrals> as_ints_;

    std::shared_ptr<ForteOptions> options_;

    std::vector<std::shared_ptr<ActiveSpaceSolver>> solvers_;

    //    /// The molecular integrals for the active space
    //    /// This object holds only the integrals for the orbital contained in the active_mo_
    //    vector.
    //    /// The one-electron integrals and scalar energy contains contributions from the
    //    /// doubly occupied orbitals specified by the core_mo_ vector.
    //    std::shared_ptr<ActiveSpaceIntegrals> as_ints_;

    //    // ==> Base Class Handles [can be changed before running compute_energy()]  <==

    //    /// The energy convergence criterion
    //    double e_convergence_ = 1.0e-12;

    //    /// The number of roots (default = 1)
    //    int nroot_ = 1;

    //    /// The root used to compute properties (zero based, default = 0)
    //    int root_ = 0;

    //    /// The maximum RDM computed (0 - 3)
    //    int max_rdm_level_ = 1;

    //    /// A variable to control printing information
    //    int print_ = 0;

    //    /// Eigenvalues
    //    psi::SharedVector evals_;

    //    /// Allocates an ActiveSpaceIntegrals object and fills it with integrals stored in ints_
    //    void make_active_space_ints();
};

///**
// * @brief make_active_space_solver Make an active space solver object
// * @param type a string that specifies the type (e.g. "FCI", "ACI", ...)
// * @param state information about the elecronic state
// * @param scf_info information about a previous SCF computation
// * @param mo_space_info orbital space information
// * @param ints an integral object
// * @param options user-provided options
// * @return a shared pointer for the base class ActiveSpaceSolver
// */
// std::unique_ptr<ActiveSpaceSolver> make_active_space_solver(
//    const std::string& type, StateInfo state, std::shared_ptr<SCFInfo> scf_info,
//    std::shared_ptr<MOSpaceInfo> mo_space_info, std::shared_ptr<ForteIntegrals> ints,
//    std::shared_ptr<ForteOptions> options);

std::vector<std::pair<StateInfo, std::vector<double>>>
make_state_weights_list(std::shared_ptr<ForteOptions> options,
                        std::shared_ptr<psi::Wavefunction> wfn);

} // namespace forte

#endif // _ms_active_space_solver_h_
