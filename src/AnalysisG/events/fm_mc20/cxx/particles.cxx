#include "particles.h"

top::top(){
  this -> type = "fm_top"; 
  this -> add_leaf("pt"    , "_pt"); 
  this -> add_leaf("eta"   , "_eta"); 
  this -> add_leaf("phi"   , "_phi"); 
  this -> add_leaf("energy", "_e"); 
  this -> add_leaf("charge", "_charge"); 

  this -> add_leaf("index"  , "_index"); 
  this -> apply_type_prefix(); 
}

top::~top(){}

particle_template* top::clone() {return (particle_template*)new top();}

void top::build(std::map<std::string, particle_template*>* prt, element_t* el){
  std::vector<top*> tps; 
  pmu(&tps, el); 
   
  std::vector<float> ch; 
  el -> get("charge", &ch); 
  std::vector<int> idx, _pdgid;  
  el -> get("index"  , &idx); 
  el -> get("pdgid"  , &_pdgid); 
  for (size_t x(0); x < tps.size(); ++x){
    tps[x] -> index   = idx[x]; 
    tps[x] -> charge  = ch[x]; 
    tps[x] -> pdgid   = 6; 

    (*prt)[std::string(tps[x] -> hash)] = tps[x]; 
  }
}

child::child(){
  this -> type = "fm_children"; 
  this -> add_leaf("pt"    , "_pt"); 
  this -> add_leaf("eta"   , "_eta"); 
  this -> add_leaf("phi"   , "_phi"); 
  this -> add_leaf("energy", "_e"); 
  this -> add_leaf("charge", "_charge"); 

  this -> add_leaf("pdgid"  , "_pdgid"); 
  this -> add_leaf("index"  , "_index"); 
  this -> apply_type_prefix(); 
}

child::~child(){}
particle_template* child::clone(){return (particle_template*)new child();}
void child::build(std::map<std::string, particle_template*>* prt, element_t* el){
  std::vector<child*> tps; 
  pmu(&tps, el); 

  std::vector<float> ch;  
  el -> get("charge", &ch); 
  
  std::vector<int> idx, _pdgid; 
  el -> get("index"  , &idx); 
  el -> get("pdgid"  , &_pdgid); 
  for (size_t x(0); x < tps.size(); ++x){
    tps[x] -> index   = idx[x]; 
    tps[x] -> charge  = ch[x]; 
    tps[x] -> pdgid   = _pdgid[x]; 
    (*prt)[std::string(tps[x] -> hash)] = tps[x]; 
  }
}

physics_detector::physics_detector(){
  this -> type = "fm_phys"; 
  this -> add_leaf("pt"    , "_pt"); 
  this -> add_leaf("eta"   , "_eta"); 
  this -> add_leaf("phi"   , "_phi"); 
  this -> add_leaf("energy", "_e"); 
 
  this -> add_leaf("cindex"    , "_cindex"); 
  this -> add_leaf("top_index", "_index"); 
  this -> add_leaf("pdgid_list", "_pdgid"); 
  this -> apply_type_prefix();
}

physics_detector::~physics_detector(){}
particle_template* physics_detector::clone(){return (particle_template*)new physics_detector();}
void physics_detector::build(std::map<std::string, particle_template*>* prt, element_t* el){
  auto lamb = [](physics_detector* msp){
    if (msp -> top_index.size() > 1){return true;}
    if (msp -> top_index[0] == -1){return false;}
    return true; 
  }; 

  auto filter = [](physics_detector* msp) {
    int n_top = msp->top_index.size();
    std::unordered_map<int, int> freq;
    for (size_t i(0); i < msp->pdgid_list.size(); ++i){
      int id = abs(msp->pdgid_list[i]);
      if ( id < 37) freq[id]++;
    }
    std::vector<std::pair<int, int>> vec(freq.begin(), freq.end());
    std::sort(vec.begin(), vec.end(),
           [](const auto& a, const auto& b) {
           if (a.second == b.second) 
             return a.first < b.first; 
           return a.second > b.second;
    });
    std::vector<int> filter_pdgid_list;

    for (int i = 0; i < n_top && i < (int)vec.size(); i++) {
      filter_pdgid_list.push_back(vec[i].first);
    }
    if (filter_pdgid_list.size()>0){
      msp-> pdgid_pertop = filter_pdgid_list;
      msp-> pdgid = filter_pdgid_list[0];
      return true;
    }  else{
      return false;
    }
  };

  std::vector<float> ch; 
  std::vector<std::vector<int>> _top_index,_cindex,pdgid_list; 

  el -> get("cindex" , &_cindex); 
  el -> get("pdgid_list", &pdgid_list); 
  el -> get("top_index", &_top_index); 

  std::vector<physics_detector*> tps = {};
  tps.reserve(_top_index.size()); 
  pmu(&tps, el); 

  for (size_t x(0); x < tps.size(); ++x){
    tps[x] -> charge    = 0; 
    tps[x] -> index     = x; 
    tps[x] -> top_index = _top_index[x]; 
    tps[x] -> pdgid_list     = pdgid_list[x]; 
    if (!lamb(tps[x])){delete tps[x]; tps[x] = nullptr;}
    if (!filter(tps[x])){delete tps[x]; tps[x] = nullptr;}
    else {(*prt)[std::string(tps[x] -> hash)] = tps[x];}
    }
}

physics_truth::physics_truth(){
  this -> type = "fm_phys_tru"; 
  this -> add_leaf("pt"    , "_pt"); 
  this -> add_leaf("eta"   , "_eta"); 
  this -> add_leaf("phi"   , "_phi"); 
  this -> add_leaf("energy", "_e"); 
 
  this -> add_leaf("cindex"    , "_cindex"); 
  this -> add_leaf("top_index", "_index"); 
  this -> add_leaf("pdgid_list", "_pdgid"); 

  this -> apply_type_prefix();
}

physics_truth::~physics_truth(){}
particle_template* physics_truth::clone(){return (particle_template*)new physics_truth();}
void physics_truth::build(std::map<std::string, particle_template*>* prt, element_t* el){
  auto lamb = [](physics_truth* msp){
    if (msp -> top_index.size() > 1){return true;}
    if (msp -> top_index[0] == -1){return false;} 
    return true; 
  }; 

  auto filter = [](physics_truth* msp) {
    int n_top = msp->top_index.size();
    std::unordered_map<int, int> freq;
    for (size_t i(0); i < msp->pdgid_list.size(); ++i){
      int id = abs(msp->pdgid_list[i]);
      if ( id < 37) freq[id]++;
    }
    std::vector<std::pair<int, int>> vec(freq.begin(), freq.end());
    std::sort(vec.begin(), vec.end(),
           [](const auto& a, const auto& b) {
               if (a.second == b.second) 
                 return a.first < b.first; 
               return a.second > b.second;
    });
    std::vector<int> filter_pdgid_list;
    for (int i = 0; i < n_top && i < (int)vec.size(); i++) {
      filter_pdgid_list.push_back(vec[i].first);
    }
    if (filter_pdgid_list.size()>0){
      msp-> pdgid_pertop = filter_pdgid_list;
      msp-> pdgid = filter_pdgid_list[0];
      return true;
    }  else{
      return false;
    }
  };

  std::vector<physics_truth*> tps = {}; 
  pmu(&tps, el); 

  std::vector<float> ch;
  std::vector<std::vector<int>> _top_index, _cindex, pdgid_list; 
  el -> get("cindex" , &_cindex); 
  el -> get("pdgid_list", &pdgid_list); 
  el -> get("top_index", &_top_index); 

  for (size_t x(0); x < tps.size(); ++x){
    tps[x] -> charge    = 0; 
    tps[x] -> index     = x; 
    tps[x] -> top_index = _top_index[x]; 
    tps[x] -> pdgid_list     = pdgid_list[x]; 

    if (!lamb(tps[x])){delete tps[x]; tps[x] = nullptr;}
    if (!filter(tps[x])){delete tps[x]; tps[x] = nullptr;}
    else {(*prt)[std::string(tps[x] -> hash)] = tps[x];}
  }
}

jet::jet(){
    this -> type = "jet"; 
    this -> add_leaf("pt"    , "_pt_NOSYS"); 
    this -> add_leaf("eta"   , "_eta"); 
    this -> add_leaf("phi"   , "_phi"); 
    this -> add_leaf("energy", "_e_NOSYS"); 

    this -> add_leaf("flav"  , "_truthflav"); 
    this -> add_leaf("parton", "_partonid"); 
    this -> apply_type_prefix(); 
}

particle_template* jet::clone(){return (particle_template*)new jet();}
void jet::build(std::map<std::string, particle_template*>* prt, element_t* el){
  std::vector<jet*> tps; 
  pmu(&tps, el); 

  std::vector<int> fl, prtn; 
  el -> get("flav"  , &fl); 
  el -> get("parton", &prtn); 
  for (int x(0); x < tps.size(); ++x){
    jet* p     = tps[x]; 
    p -> index = x; 
    p -> flav  = fl[x]; 
    p -> pdgid = prtn[x]; 
    (*prt)[std::string(p -> hash)] = p; 
  }
}

jet::~jet(){}

electron::electron(){
  this -> type = "el"; 
  this -> add_leaf("pt"    , "_pt_NOSYS"); 
  this -> add_leaf("eta"   , "_eta"); 
  this -> add_leaf("phi"   , "_phi"); 
  this -> add_leaf("energy", "_e_NOSYS"); 

  this -> add_leaf("charge"  , "_charge"); 
  this -> add_leaf("type"    , "_truthType"); 
  this -> add_leaf("origin"  , "_truthOrigin"); 

  this -> apply_type_prefix(); 
}

particle_template* electron::clone(){return (particle_template*)new electron();}
void electron::build(std::map<std::string, particle_template*>* prt, element_t* el){
  std::vector<electron*> tps; 
  pmu(&tps, el); 

  std::vector<float> _charge, d0, z0; 
  el -> get("charge"  , &_charge);
  std::vector<int> typ, org;
  el -> get("type"  , &typ); 
  el -> get("origin", &org); 

  for (int x(0); x < tps.size(); ++x){
    electron* p      = tps[x];
    p -> index       = x; 
    p -> charge      = _charge[x]; 
    p -> true_type   = typ[x]; 
    p -> true_origin = org[x]; 
    p -> pdgid       = int(11 * _charge[x]); 
    (*prt)[std::string(p -> hash)] = p;
  }
}

electron::~electron(){}

muon::muon(){
  this -> type = "mu"; 
  this -> add_leaf("pt"      , "_pt_NOSYS"); 
  this -> add_leaf("eta"     , "_eta"); 
  this -> add_leaf("phi"     , "_phi"); 
  this -> add_leaf("energy"  , "_e_NOSYS"); 

  this -> add_leaf("charge"  , "_charge"); 
  this -> add_leaf("type"    , "_truthType"); 
  this -> add_leaf("origin"  , "_truthOrigin"); 

  this -> apply_type_prefix(); 
}

particle_template* muon::clone(){return (particle_template*)new muon();}
void muon::build(std::map<std::string, particle_template*>* prt, element_t* el){
  std::vector<muon*> tps; 
  pmu(&tps, el); 

  std::vector<float> _charge, d0, z0; 
  el -> get("charge"  , &_charge);

  std::vector<int> typ, org;
  el -> get("type"  , &typ); 
  el -> get("origin", &org); 

  for (int x(0); x < tps.size(); ++x){
    muon* p       = tps[x];
    p -> index    = x; 

    p -> charge   = _charge[x]; 
    p -> true_type   = typ[x]; 
    p -> true_origin = org[x]; 
    p -> pdgid       = int(13 * _charge[x]); 
    (*prt)[std::string(p -> hash)] = p;
  }
}

muon::~muon(){}
