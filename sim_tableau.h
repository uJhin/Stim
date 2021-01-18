#ifndef SIM_TABLEAU_H
#define SIM_TABLEAU_H

#include <iostream>
#include <immintrin.h>
#include <new>
#include <cassert>
#include <sstream>
#include <functional>
#include "tableau.h"
#include <random>
#include "circuit.h"
#include "sim_vector.h"

struct SimTableau {
    Tableau inv_state;
    std::mt19937 rng;

    explicit SimTableau(size_t num_qubits);
    void ensure_large_enough_for_qubit(size_t q);

    SimVector to_vector_sim() const;
    bool is_deterministic(size_t target) const;
    bool measure(size_t q, float bias = 0.5);
    void reset(size_t target);
    std::vector<bool> measure_many(const std::vector<size_t> &targets, float bias = 0.5);
    void reset_many(const std::vector<size_t> &targets);
    static std::vector<bool> simulate(const Circuit &circuit);
    static void simulate(FILE *in, FILE *out);

    void H(size_t q);
    void H_YZ(size_t q);
    void H_XY(size_t q);
    void SQRT_X(size_t q);
    void SQRT_Y(size_t q);
    void SQRT_Z(size_t q);
    void SQRT_X_DAG(size_t q);
    void SQRT_Y_DAG(size_t q);
    void SQRT_Z_DAG(size_t q);
    void CX(size_t c, size_t t);
    void CY(size_t c, size_t t);
    void CZ(size_t c, size_t t);
    void SWAP(size_t q1, size_t q2);
    void X(size_t q);
    void Y(size_t q);
    void Z(size_t q);
    void ISWAP(size_t q1, size_t q2);
    void ISWAP_DAG(size_t q1, size_t q2);
    void XCX(size_t control, size_t target);
    void XCY(size_t control, size_t target);
    void XCZ(size_t control, size_t target);
    void YCX(size_t control, size_t target);
    void YCY(size_t control, size_t target);
    void YCZ(size_t control, size_t target);

    void func_op(const std::string &name, const std::vector<size_t> &targets);
    void tableau_op(const std::string &name, const std::vector<size_t> &targets);
    void broadcast_op(const std::string &name, const std::vector<size_t> &targets);

    std::vector<SparsePauliString> inspected_collapse(
            const std::vector<size_t> &targets);

private:
    void collapse_many(const std::vector<size_t> &targets, float bias);
    void collapse_while_transposed(
            size_t target,
            TempTransposedTableauRaii &temp_transposed,
            SparsePauliString *destabilizer_out,
            float else_bias);
};

extern const std::unordered_map<std::string, std::function<void(SimTableau &, size_t)>> SINGLE_QUBIT_GATE_FUNCS;
extern const std::unordered_map<std::string, std::function<void(SimTableau &, size_t, size_t)>> TWO_QUBIT_GATE_FUNCS;

extern const std::unordered_map<std::string, std::function<void(SimTableau &, const std::vector<size_t> &)>> BROADCAST_GATE_FUNCS;
#endif