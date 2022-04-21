
#include <algorithm>
#include <complex>
#include <iostream>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch.hpp>

#include "StateVectorCudaRaw.hpp"
#include "StateVectorRaw.hpp"
#include "cuGateCache.hpp"
#include "cuGates_host.hpp"
#include "cuda_helpers.hpp"

#include "TestHelpers.hpp"

using namespace Pennylane;
using namespace CUDA;

TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyRX", "[StateVectorCudaRaw_Param]",
                   double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 1;
    SVDataGPURaw<TestType> svdat{num_qubits};

    const std::vector<TestType> angles{{0.1}, {0.6}};
    std::vector<std::vector<cp_t>> expected_results{
        std::vector<cp_t>{{0.9987502603949663, 0.0},
                          {0.0, -0.04997916927067834}},
        std::vector<cp_t>{{0.9553364891256061, 0.0}, {0, -0.2955202066613395}},
        std::vector<cp_t>{{0.49757104789172696, 0.0}, {0, -0.867423225594017}}};

    std::vector<std::vector<cp_t>> expected_results_adj{
        std::vector<cp_t>{{0.9987502603949663, 0.0},
                          {0.0, 0.04997916927067834}},
        std::vector<cp_t>{{0.9553364891256061, 0.0}, {0, 0.2955202066613395}},
        std::vector<cp_t>{{0.49757104789172696, 0.0}, {0, 0.867423225594017}}};

    const auto init_state = svdat.sv.getDataVector();
    SECTION("adj = false") {
        SECTION("Apply directly") {
            for (size_t index = 0; index < angles.size(); index++) {
                SVDataGPURaw<TestType> svdat_direct{num_qubits, init_state};

                svdat_direct.cuda_sv_raw.applyRX({0}, false, angles[index]);
                svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
                CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                    expected_results[index], 1e-7));
            }
        }
        SECTION("Apply using dispatcher") {
            for (size_t index = 0; index < angles.size(); index++) {
                SVDataGPURaw<TestType> svdat_dispatch{num_qubits, init_state};
                svdat_dispatch.cuda_sv_raw.applyOperation("RX", {0}, false,
                                                          {angles[index]});
                svdat_dispatch.cuda_sv_raw.CopyGpuDataToHost(svdat_dispatch.sv);
                CHECK(isApproxEqual(svdat_dispatch.sv.getDataVector(),
                                    expected_results[index], 1e-7));
            }
        }
    }
    SECTION("adj = true") {
        SECTION("Apply directly") {
            for (size_t index = 0; index < angles.size(); index++) {
                SVDataGPURaw<TestType> svdat_direct{num_qubits, init_state};
                svdat_direct.cuda_sv_raw.applyRX({0}, true, {angles[index]});
                svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
                CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                    expected_results_adj[index], 1e-7));
            }
        }
        SECTION("Apply using dispatcher") {
            for (size_t index = 0; index < angles.size(); index++) {
                SVDataGPURaw<TestType> svdat_dispatch{num_qubits, init_state};
                svdat_dispatch.cuda_sv_raw.applyOperation("RX", {0}, true,
                                                          {angles[index]});
                svdat_dispatch.cuda_sv_raw.CopyGpuDataToHost(svdat_dispatch.sv);
                CHECK(isApproxEqual(svdat_dispatch.sv.getDataVector(),
                                    expected_results_adj[index], 1e-7));
            }
        }
    }
}

TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyRY", "[StateVectorCudaRaw_Param]",
                   float, double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 1;
    SVDataGPURaw<TestType> svdat{num_qubits};

    const std::vector<TestType> angles{0.2, 0.7, 2.9};
    std::vector<std::vector<cp_t>> expected_results{
        std::vector<cp_t>{{0.8731983044562817, 0.04786268954660339},
                          {0.0876120655431924, -0.47703040785184303}},
        std::vector<cp_t>{{0.8243771119105122, 0.16439396602553008},
                          {0.3009211363333468, -0.45035926880694604}},
        std::vector<cp_t>{{0.10575112905629831, 0.47593196040758534},
                          {0.8711876098966215, -0.0577721051072477}}};
    std::vector<std::vector<cp_t>> expected_results_adj{
        std::vector<cp_t>{{0.8731983044562817, -0.04786268954660339},
                          {-0.0876120655431924, -0.47703040785184303}},
        std::vector<cp_t>{{0.8243771119105122, -0.16439396602553008},
                          {-0.3009211363333468, -0.45035926880694604}},
        std::vector<cp_t>{{0.10575112905629831, -0.47593196040758534},
                          {-0.8711876098966215, -0.0577721051072477}}};

    const std::vector<cp_t> init_state{{0.8775825618903728, 0.0},
                                       {0.0, -0.47942553860420306}};
    SECTION("adj = false") {
        SECTION("Apply directly") {
            for (size_t index = 0; index < angles.size(); index++) {
                SVDataGPURaw<TestType> svdat_direct{num_qubits, init_state};

                svdat_direct.cuda_sv_raw.applyRY({0}, false, {angles[index]});
                svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
                CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                    expected_results[index], 1e-6));
            }
        }
        SECTION("Apply using dispatcher") {
            for (size_t index = 0; index < angles.size(); index++) {
                SVDataGPURaw<TestType> svdat_dispatch{num_qubits, init_state};
                svdat_dispatch.cuda_sv_raw.applyOperation("RY", {0}, false,
                                                          {angles[index]});
                svdat_dispatch.cuda_sv_raw.CopyGpuDataToHost(svdat_dispatch.sv);
                CHECK(isApproxEqual(svdat_dispatch.sv.getDataVector(),
                                    expected_results[index], 1e-6));
            }
        }
    }
    SECTION("adj = true") {
        SECTION("Apply directly") {
            for (size_t index = 0; index < angles.size(); index++) {
                SVDataGPURaw<TestType> svdat_direct{num_qubits, init_state};

                svdat_direct.cuda_sv_raw.applyRY({0}, true, {angles[index]});
                svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);

                CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                    expected_results_adj[index], 1e-6));
            }
        }
        SECTION("Apply using dispatcher") {
            for (size_t index = 0; index < angles.size(); index++) {
                SVDataGPURaw<TestType> svdat_dispatch{num_qubits, init_state};
                svdat_dispatch.cuda_sv_raw.applyOperation("RY", {0}, true,
                                                          {angles[index]});

                svdat_dispatch.cuda_sv_raw.CopyGpuDataToHost(svdat_dispatch.sv);
                CHECK(isApproxEqual(svdat_dispatch.sv.getDataVector(),
                                    expected_results_adj[index], 1e-6));
            }
        }
    }
}

TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyRZ", "[StateVectorCudaRaw_Param]",
                   float, double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 3;
    SVDataGPURaw<TestType> svdat{num_qubits};

    // Test using |+++> state
    svdat.cuda_sv_raw.applyOperation({{"Hadamard"}, {"Hadamard"}, {"Hadamard"}},
                                     {{0}, {1}, {2}},
                                     {{false}, {false}, {false}});
    svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);
    const std::vector<TestType> angles{0.2, 0.7, 2.9};
    const cp_t coef(1.0 / (2 * std::sqrt(2)), 0);

    std::vector<std::vector<cp_t>> rz_data;
    rz_data.reserve(angles.size());
    for (auto &a : angles) {
        rz_data.push_back(Gates::getRZ<TestType>(a));
    }

    std::vector<std::vector<cp_t>> expected_results = {
        {rz_data[0][0], rz_data[0][0], rz_data[0][0], rz_data[0][0],
         rz_data[0][3], rz_data[0][3], rz_data[0][3], rz_data[0][3]},
        {
            rz_data[1][0],
            rz_data[1][0],
            rz_data[1][3],
            rz_data[1][3],
            rz_data[1][0],
            rz_data[1][0],
            rz_data[1][3],
            rz_data[1][3],
        },
        {rz_data[2][0], rz_data[2][3], rz_data[2][0], rz_data[2][3],
         rz_data[2][0], rz_data[2][3], rz_data[2][0], rz_data[2][3]}};

    for (auto &vec : expected_results) {
        scaleVector(vec, coef);
    }

    const auto init_state = svdat.sv.getDataVector();
    SECTION("Apply directly") {
        for (size_t index = 0; index < num_qubits; index++) {
            SVDataGPURaw<TestType> svdat_direct{num_qubits, init_state};

            svdat_direct.cuda_sv_raw.applyRZ({index}, false, {angles[index]});
            svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
            CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                expected_results[index], 1e-6));
        }
    }
    SECTION("Apply using dispatcher") {
        for (size_t index = 0; index < num_qubits; index++) {
            SVDataGPURaw<TestType> svdat_dispatch{num_qubits, init_state};
            svdat_dispatch.cuda_sv_raw.applyOperation("RZ", {index}, false,
                                                      {angles[index]});

            svdat_dispatch.cuda_sv_raw.CopyGpuDataToHost(svdat_dispatch.sv);
            CHECK(isApproxEqual(svdat_dispatch.sv.getDataVector(),
                                expected_results[index], 1e-6));
        }
    }
}

TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyPhaseShift",
                   "[StateVectorCudaRaw_Param]", float, double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 3;
    SVDataGPURaw<TestType> svdat{num_qubits};

    // Test using |+++> state
    svdat.cuda_sv_raw.applyOperation({{"Hadamard"}, {"Hadamard"}, {"Hadamard"}},
                                     {{0}, {1}, {2}},
                                     {{false}, {false}, {false}});
    svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);

    const std::vector<TestType> angles{0.3, 0.8, 2.4};
    const cp_t coef(1.0 / (2 * std::sqrt(2)), 0);

    std::vector<std::vector<cp_t>> ps_data;
    ps_data.reserve(angles.size());
    for (auto &a : angles) {
        ps_data.push_back(Gates::getPhaseShift<TestType>(a));
    }

    std::vector<std::vector<cp_t>> expected_results = {
        {ps_data[0][0], ps_data[0][0], ps_data[0][0], ps_data[0][0],
         ps_data[0][3], ps_data[0][3], ps_data[0][3], ps_data[0][3]},
        {
            ps_data[1][0],
            ps_data[1][0],
            ps_data[1][3],
            ps_data[1][3],
            ps_data[1][0],
            ps_data[1][0],
            ps_data[1][3],
            ps_data[1][3],
        },
        {ps_data[2][0], ps_data[2][3], ps_data[2][0], ps_data[2][3],
         ps_data[2][0], ps_data[2][3], ps_data[2][0], ps_data[2][3]}};

    for (auto &vec : expected_results) {
        scaleVector(vec, coef);
    }

    const auto init_state = svdat.sv.getDataVector();
    SECTION("Apply directly") {
        for (size_t index = 0; index < num_qubits; index++) {
            SVDataGPURaw<TestType> svdat_direct{num_qubits, init_state};

            svdat_direct.cuda_sv_raw.applyPhaseShift({index}, false,
                                                     {angles[index]});
            svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
            CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                expected_results[index]));
        }
    }
    SECTION("Apply using dispatcher") {
        for (size_t index = 0; index < num_qubits; index++) {
            SVDataGPURaw<TestType> svdat_dispatch{num_qubits, init_state};
            svdat_dispatch.cuda_sv_raw.applyOperation("PhaseShift", {index},
                                                      false, {angles[index]});
            svdat_dispatch.cuda_sv_raw.CopyGpuDataToHost(svdat_dispatch.sv);
            CHECK(isApproxEqual(svdat_dispatch.sv.getDataVector(),
                                expected_results[index]));
        }
    }
}

TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyControlledPhaseShift",
                   "[StateVectorCudaRaw_Param]", float, double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 3;
    SVDataGPURaw<TestType> svdat{num_qubits};

    // Test using |+++> state
    svdat.cuda_sv_raw.applyOperation({{"Hadamard"}, {"Hadamard"}, {"Hadamard"}},
                                     {{0}, {1}, {2}},
                                     {{false}, {false}, {false}});

    svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);
    const std::vector<TestType> angles{0.3, 2.4};
    const cp_t coef(1.0 / (2 * std::sqrt(2)), 0);

    std::vector<std::vector<cp_t>> ps_data;
    ps_data.reserve(angles.size());
    for (auto &a : angles) {
        ps_data.push_back(Gates::getPhaseShift<TestType>(a));
    }

    std::vector<std::vector<cp_t>> expected_results = {
        {ps_data[0][0], ps_data[0][0], ps_data[0][0], ps_data[0][0],
         ps_data[0][0], ps_data[0][0], ps_data[0][3], ps_data[0][3]},
        {ps_data[1][0], ps_data[1][0], ps_data[1][0], ps_data[1][3],
         ps_data[1][0], ps_data[1][0], ps_data[1][0], ps_data[1][3]}};

    for (auto &vec : expected_results) {
        scaleVector(vec, coef);
    }

    const auto init_state = svdat.sv.getDataVector();
    SECTION("Apply directly") {
        SVDataGPURaw<TestType> svdat_direct{num_qubits, init_state};

        svdat_direct.cuda_sv_raw.applyControlledPhaseShift({0, 1}, false,
                                                           {angles[0]});
        svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
        CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                            expected_results[0]));
    }
    SECTION("Apply using dispatcher") {
        SVDataGPURaw<TestType> svdat_dispatch{num_qubits, init_state};
        svdat_dispatch.cuda_sv_raw.applyOperation("ControlledPhaseShift",
                                                  {1, 2}, false, {angles[1]});
        svdat_dispatch.cuda_sv_raw.CopyGpuDataToHost(svdat_dispatch.sv);
        CHECK(isApproxEqual(svdat_dispatch.sv.getDataVector(),
                            expected_results[1]));
    }
}

TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyRot", "[StateVectorCudaRaw_Param]",
                   float, double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 3;
    SVDataGPURaw<TestType> svdat{num_qubits};

    const std::vector<std::vector<TestType>> angles{
        std::vector<TestType>{0.3, 0.8, 2.4},
        std::vector<TestType>{0.5, 1.1, 3.0},
        std::vector<TestType>{2.3, 0.1, 0.4}};

    std::vector<std::vector<cp_t>> expected_results{
        std::vector<cp_t>(0b1 << num_qubits),
        std::vector<cp_t>(0b1 << num_qubits),
        std::vector<cp_t>(0b1 << num_qubits)};

    for (size_t i = 0; i < angles.size(); i++) {
        const auto rot_mat =
            Gates::getRot<TestType>(angles[i][0], angles[i][1], angles[i][2]);
        expected_results[i][0] = rot_mat[0];
        expected_results[i][0b1 << (num_qubits - i - 1)] = rot_mat[2];
    }

    SECTION("Apply directly") {
        for (size_t index = 0; index < num_qubits; index++) {
            SVDataGPURaw<TestType> svdat_direct{num_qubits};

            svdat_direct.cuda_sv_raw.applyRot({index}, false, angles[index][0],
                                              angles[index][1],
                                              angles[index][2]);
            svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
            CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                expected_results[index], 1e-6));
        }
    }
    SECTION("Apply using dispatcher") {
        for (size_t index = 0; index < num_qubits; index++) {
            SVDataGPURaw<TestType> svdat_dispatch{num_qubits};
            svdat_dispatch.cuda_sv_raw.applyOperation("Rot", {index}, false,
                                                      angles[index]);
            svdat_dispatch.cuda_sv_raw.CopyGpuDataToHost(svdat_dispatch.sv);
            CHECK(isApproxEqual(svdat_dispatch.sv.getDataVector(),
                                expected_results[index], 1e-6));
        }
    }
}

TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyCRot",
                   "[StateVectorCudaRaw_Param]", float, double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 3;
    SVDataGPURaw<TestType> svdat{num_qubits};

    const std::vector<TestType> angles{0.3, 0.8, 2.4};

    std::vector<cp_t> expected_results(8);
    const auto rot_mat =
        Gates::getRot<TestType>(angles[0], angles[1], angles[2]);
    expected_results[0b1 << (num_qubits - 1)] = rot_mat[0];
    expected_results[(0b1 << num_qubits) - 2] = rot_mat[2];

    const auto init_state = svdat.sv.getDataVector();

    SECTION("Apply directly") {
        SECTION("CRot0,1 |000> -> |000>") {
            SVDataGPURaw<TestType> svdat_direct{num_qubits};
            svdat_direct.cuda_sv_raw.applyCRot({0, 1}, false, angles[0],
                                               angles[1], angles[2]);
            svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
            CHECK(isApproxEqual(svdat_direct.sv.getDataVector(), init_state));
        }
        SECTION("CRot0,1 |100> -> |1>(a|0>+b|1>)|0>") {
            SVDataGPURaw<TestType> svdat_direct{num_qubits};
            svdat_direct.cuda_sv_raw.applyOperation("PauliX", {0});

            svdat_direct.cuda_sv_raw.applyCRot({0, 1}, false, angles[0],
                                               angles[1], angles[2]);
            svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
            CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                expected_results, 1e-6));
        }
    }
    SECTION("Apply using dispatcher") {
        SECTION("CRot0,1 |100> -> |1>(a|0>+b|1>)|0>") {
            SVDataGPURaw<TestType> svdat_direct{num_qubits};
            svdat_direct.cuda_sv_raw.applyOperation("PauliX", {0});

            svdat_direct.cuda_sv_raw.applyOperation("CRot", {0, 1}, false,
                                                    angles);
            svdat_direct.cuda_sv_raw.CopyGpuDataToHost(svdat_direct.sv);
            CHECK(isApproxEqual(svdat_direct.sv.getDataVector(),
                                expected_results, 1e-6));
        }
    }
}

// NOLINTNEXTLINE: Avoid complexity errors
TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyOperation 1 wire",
                   "[StateVectorCudaRaw_Param]", float, double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 5;

    // Note: gates are defined as right-to-left order

    SECTION("Apply XZ gate") {
        const std::vector<cp_t> xz_gate{
            cuUtil::ZERO<cp_t>(), cuUtil::ONE<cp_t>(), -cuUtil::ONE<cp_t>(),
            cuUtil::ZERO<cp_t>()};

        SECTION("Apply using dispatcher") {
            SVDataGPURaw<TestType> svdat{num_qubits};
            SVDataGPURaw<TestType> svdat_expected{num_qubits};

            for (size_t index = 0; index < num_qubits; index++) {
                svdat_expected.cuda_sv_raw.applyOperation(
                    {{"PauliX"}, {"PauliZ"}}, {{index}, {index}},
                    {false, false});

                svdat.cuda_sv_raw.applyOperation_std("XZ", {index}, false,
                                                     {0.0}, xz_gate);
            }
            svdat_expected.cuda_sv_raw.CopyGpuDataToHost(svdat_expected.sv);
            svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);

            CHECK(isApproxEqual(svdat.sv.getDataVector(),
                                svdat_expected.sv.getDataVector()));
        }
    }
    SECTION("Apply ZX gate") {
        const std::vector<cp_t> zx_gate{
            cuUtil::ZERO<cp_t>(), -cuUtil::ONE<cp_t>(), cuUtil::ONE<cp_t>(),
            cuUtil::ZERO<cp_t>()};

        SECTION("Apply using dispatcher") {
            SVDataGPURaw<TestType> svdat{num_qubits};
            SVDataGPURaw<TestType> svdat_expected{num_qubits};

            for (size_t index = 0; index < num_qubits; index++) {
                svdat_expected.cuda_sv_raw.applyOperation(
                    {{"PauliZ"}, {"PauliX"}}, {{index}, {index}},
                    {false, false});
                svdat_expected.cuda_sv_raw.CopyGpuDataToHost(svdat_expected.sv);
                svdat.cuda_sv_raw.applyOperation_std("ZX", {index}, false,
                                                     {0.0}, zx_gate);
                svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);
            }
            CHECK(isApproxEqual(svdat.sv.getDataVector(),
                                svdat_expected.sv.getDataVector()));
        }
    }
    SECTION("Apply XY gate") {
        const std::vector<cp_t> xy_gate{
            -cuUtil::IMAG<cp_t>(), cuUtil::ZERO<cp_t>(), cuUtil::ZERO<cp_t>(),
            cuUtil::IMAG<cp_t>()};

        SECTION("Apply using dispatcher") {
            SVDataGPURaw<TestType> svdat{num_qubits};
            SVDataGPURaw<TestType> svdat_expected{num_qubits};

            for (size_t index = 0; index < num_qubits; index++) {
                svdat_expected.cuda_sv_raw.applyOperation(
                    {{"PauliX"}, {"PauliY"}}, {{index}, {index}},
                    {false, false});
                svdat_expected.cuda_sv_raw.CopyGpuDataToHost(svdat_expected.sv);
                svdat.cuda_sv_raw.applyOperation_std("XY", {index}, false,
                                                     {0.0}, xy_gate);
                svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);
            }
            CHECK(isApproxEqual(svdat.sv.getDataVector(),
                                svdat_expected.sv.getDataVector()));
        }
    }
    SECTION("Apply YX gate") {
        const std::vector<cp_t> yx_gate{
            cuUtil::IMAG<cp_t>(), cuUtil::ZERO<cp_t>(), cuUtil::ZERO<cp_t>(),
            -cuUtil::IMAG<cp_t>()};

        SECTION("Apply using dispatcher") {
            SVDataGPURaw<TestType> svdat{num_qubits};
            SVDataGPURaw<TestType> svdat_expected{num_qubits};

            for (size_t index = 0; index < num_qubits; index++) {
                svdat_expected.cuda_sv_raw.applyOperation(
                    {{"PauliY"}, {"PauliX"}}, {{index}, {index}},
                    {false, false});
                svdat_expected.cuda_sv_raw.CopyGpuDataToHost(svdat_expected.sv);
                svdat.cuda_sv_raw.applyOperation_std("YX", {index}, false,
                                                     {0.0}, yx_gate);
                svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);
            }
            CHECK(isApproxEqual(svdat.sv.getDataVector(),
                                svdat_expected.sv.getDataVector()));
        }
    }
    SECTION("Apply YZ gate") {
        const std::vector<cp_t> yz_gate{
            cuUtil::ZERO<cp_t>(), -cuUtil::IMAG<cp_t>(), -cuUtil::IMAG<cp_t>(),
            cuUtil::ZERO<cp_t>()};

        SECTION("Apply using dispatcher") {
            SVDataGPURaw<TestType> svdat{num_qubits};
            SVDataGPURaw<TestType> svdat_expected{num_qubits};

            for (size_t index = 0; index < num_qubits; index++) {
                svdat_expected.cuda_sv_raw.applyOperation(
                    {{"PauliY"}, {"PauliZ"}}, {{index}, {index}},
                    {false, false});
                svdat_expected.cuda_sv_raw.CopyGpuDataToHost(svdat_expected.sv);
                svdat.cuda_sv_raw.applyOperation_std("YZ", {index}, false,
                                                     {0.0}, yz_gate);
                svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);
            }
            CHECK(isApproxEqual(svdat.sv.getDataVector(),
                                svdat_expected.sv.getDataVector()));
        }
    }
    SECTION("Apply ZY gate") {
        const std::vector<cp_t> zy_gate{
            cuUtil::ZERO<cp_t>(), cuUtil::IMAG<cp_t>(), cuUtil::IMAG<cp_t>(),
            cuUtil::ZERO<cp_t>()};

        SECTION("Apply using dispatcher") {
            SVDataGPURaw<TestType> svdat{num_qubits};
            SVDataGPURaw<TestType> svdat_expected{num_qubits};

            for (size_t index = 0; index < num_qubits; index++) {
                svdat_expected.cuda_sv_raw.applyOperation(
                    {{"PauliZ"}, {"PauliY"}}, {{index}, {index}},
                    {false, false});
                svdat_expected.cuda_sv_raw.CopyGpuDataToHost(svdat_expected.sv);
                svdat.cuda_sv_raw.applyOperation_std("ZY", {index}, false,
                                                     {0.0}, zy_gate);
                svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);
            }
            CHECK(isApproxEqual(svdat.sv.getDataVector(),
                                svdat_expected.sv.getDataVector()));
        }
    }
}

TEMPLATE_TEST_CASE("StateVectorCudaRaw::applyOperation multiple wires",
                   "[StateVectorCudaRaw_Param]", float, double) {
    using cp_t = std::complex<TestType>;
    const size_t num_qubits = 3;

    SVDataGPURaw<TestType> svdat_init{num_qubits};
    svdat_init.cuda_sv_raw.applyOperation(
        {{"Hadamard"}, {"Hadamard"}, {"Hadamard"}}, {{0}, {1}, {2}},
        {false, false, false});

    svdat_init.cuda_sv_raw.CopyGpuDataToHost(svdat_init.sv);
    const auto cz_gate = cuGates::getCZ<cp_t>();
    const auto tof_gate = cuGates::getToffoli<cp_t>();
    const auto arb_gate = cuGates::getToffoli<cp_t>();

    SECTION("Apply CZ gate") {
        SVDataGPURaw<TestType> svdat{num_qubits, svdat_init.sv.getDataVector()};
        SVDataGPURaw<TestType> svdat_expected{num_qubits,
                                              svdat_init.sv.getDataVector()};

        svdat_expected.cuda_sv_raw.applyOperation(
            {{"Hadamard"}, {"CNOT"}, {"Hadamard"}}, {{1}, {0, 1}, {1}},
            {false, false, false});

        svdat_expected.cuda_sv_raw.CopyGpuDataToHost(svdat_expected.sv);
        svdat.cuda_sv_raw.applyOperation_std("CZmat", {0, 1}, false, {0.0},
                                             cz_gate);
        svdat.cuda_sv_raw.CopyGpuDataToHost(svdat.sv);
        CHECK(isApproxEqual(svdat.sv.getDataVector(),
                            svdat_expected.sv.getDataVector()));
    }
}
