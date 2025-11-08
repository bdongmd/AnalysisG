#ifndef EVENTS_PARTICLES_FM_MC20_H
#define EVENTS_PARTICLES_FM_MC20_H

#include <templates/particle_template.h>

template <typename g>
void pmu(std::vector<g*>* out, element_t* el){
  std::vector<float> pt, eta, phi, en;
  el -> get("pt",     &pt);
  el -> get("eta",    &eta);
  el -> get("phi",    &phi);
  el -> get("energy", &en);
  
  for (size_t x(0); x < pt.size(); ++x){
    g* prt = new g();
    prt -> pt  = pt[x];
    prt -> eta = eta[x];
    prt -> phi = phi[x];
    prt -> e   = en[x];
    out -> push_back(prt);
  }
}

class top: public particle_template {
  public:
    top();
    virtual ~top();

    int top_index = -1;
    particle_template* clone() override;
    void build(std::map<std::string, particle_template*>* prt, element_t* el) override;
};

class child: public particle_template {
  public:
    child();
    virtual ~child();

    std::vector<int> top_index = {};
    particle_template* clone() override;
    void build(std::map<std::string, particle_template*>* prt, element_t* el) override;
};

class physics_truth: public particle_template {
  public:
    physics_truth();
    virtual ~physics_truth();

    std::vector<int> top_index = {};
    std::vector<int> pdgid_list = {};

    particle_template* clone() override;
    void build(std::map<std::string, particle_template*>* prt, element_t* el) override;
};

class physics_detector: public particle_template {
  public:
    physics_detector();
    virtual ~physics_detector();

    std::vector<int> top_index = {};
    std::vector<int> pdgid_list = {};

    particle_template* clone() override;
    void build(std::map<std::string, particle_template*>* prt, element_t* el) override;
};

class electron: public particle_template {
  public:
    electron();
    virtual ~electron();

    int true_type = 0;
    int true_origin = 0;
    std::vector<int> top_index = {};

    particle_template* clone() override;
    void build(std::map<std::string, particle_template*>* prt, element_t* el) override;
};

class muon: public particle_template {
  public:
    muon();
    virtual ~muon();

    int true_type = 0;
    int true_origin = 0;
    std::vector<int> top_index = {};

    particle_template* clone() override;
    void build(std::map<std::string, particle_template*>* prt, element_t* el) override;
};

class jet: public particle_template {
  public:
    jet();
    virtual ~jet();

    int flav;
    int label;
    std::vector<int> top_index = {};

    particle_template* clone() override;
    void build(std::map<std::string, particle_template*>* prt, element_t* el) override;
};

#endif
