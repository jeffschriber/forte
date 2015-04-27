#ifndef _capriccio_string_lists_
#define _capriccio_string_lists_

#include <map>
#include <vector>
#include <utility>
#include "libmints/dimension.h"

#include "boost/shared_ptr.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_comparison.hpp"
#include "binary_graph.hpp"

namespace psi{ namespace libadaptive{

struct DetAddress {
    int alfa_sym;
    size_t alfa_string;
    size_t beta_string;
    DetAddress(const int& alfa_sym_, const size_t& alfa_string_, const size_t& beta_string_)
        : alfa_sym(alfa_sym_), alfa_string(alfa_string_), beta_string(beta_string_) {}
};

struct StringSubstitution {
    short sign;
    size_t I;
    size_t J;
    StringSubstitution(const int& sign_, const size_t& I_, const size_t& J_) : sign(sign_), I(I_), J(J_) {}
};

typedef boost::shared_ptr<BinaryGraph> GraphPtr;
typedef std::map<boost::tuple<size_t,size_t,int>,std::vector<StringSubstitution> > VOList;
typedef std::map<boost::tuple<size_t,size_t,size_t,size_t,int>,std::vector<StringSubstitution> > VOVOList;
typedef std::map<boost::tuple<size_t,size_t,size_t,size_t,int>,std::vector<StringSubstitution> > VVOOList;
typedef std::map<boost::tuple<int,size_t,int>,std::vector<StringSubstitution> > OOList;
typedef std::pair<int,int>          Pair;
typedef std::vector<Pair>           PairList;
typedef std::vector<PairList>       NNList;

// Enum for selecting substitution lists with one or one and two substitutions
enum RequiredLists {
    oneSubstituition,twoSubstituitionVVOO,twoSubstituitionVOVO
};

/**
 * @brief The StringLists class
 *
 * This class computes mappings between alpha/beta strings
 *
 * @param cmopi The number of correlated MOs per irrep
 * @param cmo_to_mo Maps the correlated MOs to all the MOs
 * @param na The number of alpha orbitals
 * @param nb The number of beta orbitals
 */
class StringLists {
public:


    // ==> Constructor and Destructor <==

    StringLists(RequiredLists required_lists, Dimension cmopi, std::vector<size_t> core_mo, std::vector<size_t> cmo_to_mo, size_t na, size_t nb);
    ~StringLists() {}


    // ==> Class Public Functions <==

    size_t na() const {return na_;}
    size_t nb() const {return nb_;}
    size_t ncmo() const {return ncmo_;}
    Dimension cmopi() const {return cmopi_;}
    std::vector<size_t> cmopi_offset() const {return cmopi_offset_;}
    std::vector<size_t> fomo_to_mo() const {return fomo_to_mo_;}
    std::vector<size_t> cmo_to_mo() const {return cmo_to_mo_;}
    //  int get_pairpi(int h) const {return pairpi[h];}
    //  std::vector<int> get_cmos() const {return cmos;}
    //  std::vector<int> get_cmos_offset() const {return cmos_offset;}
    //  std::vector<int> get_cmos_to_mos() const {return cmos_to_mos;}

    GraphPtr alfa_graph() {return alfa_graph_;}
    GraphPtr beta_graph() {return beta_graph_;}

    std::vector<StringSubstitution>& get_alfa_vo_list(size_t p, size_t q,int h);
    std::vector<StringSubstitution>& get_beta_vo_list(size_t p, size_t q,int h);
    std::vector<StringSubstitution>& get_alfa_vovo_list(size_t p, size_t q,size_t r, size_t s,int h);
    std::vector<StringSubstitution>& get_beta_vovo_list(size_t p, size_t q,size_t r, size_t s,int h);

    std::vector<StringSubstitution>& get_alfa_oo_list(int pq_sym, size_t pq, int h);
    std::vector<StringSubstitution>& get_beta_oo_list(int pq_sym, size_t pq, int h);

    std::vector<StringSubstitution>& get_alfa_vvoo_list(size_t p, size_t q, size_t r, size_t s, int h);
    std::vector<StringSubstitution>& get_beta_vvoo_list(size_t p, size_t q, size_t r, size_t s, int h);

    //  Pair get_nn_list_pair(int h,int n) const {return nn_list[h][n];}

    //  size_t get_nalfa_strings() const {return nas;}
    //  size_t get_nbeta_strings() const {return nbs;}
private:


    // ==> Class Data <==

    /// Flag for the type of list required
    RequiredLists  required_lists_;
    /// The number of irreps
    size_t nirrep_;
    /// The total number of correlated molecular orbitals
    size_t ncmo_;
    /// The number of correlated molecular orbitals per irrep
    Dimension  cmopi_;
    /// The offset array for cmopi_
    std::vector<size_t> cmopi_offset_;
    /// The mapping between correlated molecular orbitals and all orbitals
    std::vector<size_t> cmo_to_mo_;
    /// The mapping between frozen occupied molecular orbitals and all orbitals
    std::vector<size_t> fomo_to_mo_;
    /// The number of alpha electrons
    size_t na_;
    /// The number of beta electrons
    size_t nb_;
    /// The number of alpha strings
    size_t nas_;
    /// The number of beta strings
    size_t nbs_;
    /// The number of FCI determinants
    size_t nfcidets_;
    /// The total number of orbital pairs per irrep
    std::vector<int> pairpi_;
    /// The offset array for pairpi
    std::vector<int> pair_offset_;

    // String lists
    /// The pair string list
    NNList    nn_list;
    /// The VO string list
    VOList    alfa_vo_list;
    VOList    beta_vo_list;
    /// The OO string list
    OOList    alfa_oo_list;
    OOList    beta_oo_list;
    /// The VOVO string list
    VOVOList  alfa_vovo_list;
    VOVOList  beta_vovo_list;
    /// The VVIO string list
    VVOOList  alfa_vvoo_list;
    VVOOList  beta_vvoo_list;
    
    // Graphs
    /// The alpha string graph
    GraphPtr  alfa_graph_;
    /// The beta string graph
    GraphPtr  beta_graph_;
    /// The orbital pair graph
    GraphPtr  pair_graph_;

    // Timers
    double vo_list_timer = 0.0;
    double nn_list_timer = 0.0;
    double oo_list_timer = 0.0;
    double vovo_list_timer = 0.0;
    double vvoo_list_timer = 0.0;


    // ==> Class Functions <==

    void startup();

    void make_pair_list(GraphPtr graph,NNList& list);

    void make_vo_list(GraphPtr graph,VOList& list);
    void make_vo(GraphPtr graph,VOList& list,int p, int q);

    void make_oo_list(GraphPtr graph,OOList& list);
    void make_oo(GraphPtr graph,OOList& list,int pq_sym,size_t pq);

    void make_vovo_list(GraphPtr graph,VOVOList& list);
    void make_VOVO(GraphPtr graph,VOVOList& list,int p, int q,int r, int s);

    void make_vvoo_list(GraphPtr graph,VVOOList& list);
    void make_vvoo(GraphPtr graph,VVOOList& list,int p, int q,int r, int s);
};

}}
#endif  // _capriccio_string_lists_
