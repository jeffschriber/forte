#!/usr/bin/env python
# -*- coding: utf-8 -*-


def test_ccsd():
    """Test CCSD on H2 using RHF/DZ orbitals"""

    import pytest
    import forte.proc.scc as scc
    import forte
    import psi4

    ref_energy = -1.126712715716011  # CCSD = FCI energy from psi4

    geom = """
     H
     H 1 1.0
    """

    scf_energy, psi4_wfn = forte.utils.psi4_scf(geom, basis="DZ", reference="RHF")
    data = forte.modules.ObjectsUtilPsi4(ref_wnf=psi4_wfn).run()
    calc_data = scc.run_cc(
        data.as_ints, data.scf_info, data.mo_space_info, cc_type="cc", max_exc=2, e_convergence=1.0e-11
    )

    psi4.core.clean()

    energy = calc_data[-1][1]

    print(f"  HF energy:   {scf_energy}")
    print(f"  CCSD energy: {energy}")
    print(f"  E - Eref:    {energy - ref_energy}")

    assert energy == pytest.approx(ref_energy, 1.0e-11)


if __name__ == "__main__":
    test_ccsd()
