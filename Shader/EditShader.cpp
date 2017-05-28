#include "EditShader.hpp"
#include <iscore/model/path/PathSerialization.hpp>

namespace Shader
{
EditShader::EditShader(
        const Shader::ProcessModel& model
    , QString n):
    m_model{model}
  , m_old{model.fragment()}
  , m_new{n}
{
}

void EditShader::undo() const
{
    Shader::ProcessModel& proc = m_model.find();
    proc.setFragment(m_old);
}

void EditShader::redo() const
{
    Shader::ProcessModel& proc = m_model.find();
    proc.setFragment(m_new);
}

void EditShader::serializeImpl(DataStreamInput& s) const
{
    s << m_model << m_old << m_new;
}

void EditShader::deserializeImpl(DataStreamOutput& s)
{
    s >> m_model >> m_old >> m_new;
}



AddInlet::AddInlet(
        const Process::DataflowProcess& model):
    m_model{model}
{
}

void AddInlet::undo() const
{
  Process::DataflowProcess& proc = m_model.find();
  auto inlets = proc.inlets();
  inlets.resize(inlets.size() - 1);
  proc.setInlets(std::move(inlets));
}

void AddInlet::redo() const
{
  Process::DataflowProcess& proc = m_model.find();
  auto inlets = proc.inlets();
  inlets.push_back(Process::Port{Process::PortType::Message, "New port", State::AddressAccessor{}});
  proc.setInlets(std::move(inlets));
}

void AddInlet::serializeImpl(DataStreamInput& s) const
{
    s << m_model;
}

void AddInlet::deserializeImpl(DataStreamOutput& s)
{
    s >> m_model;
}


SetInletData::SetInletData(
        const Process::DataflowProcess& model,
    QString n, int pos):
    m_model{model}
  , m_old{model.inlets()[pos].customData}
  , m_new{n}
  , m_pos{pos}
{
}

void SetInletData::undo() const
{
  Process::DataflowProcess& proc = m_model.find();
  auto inlets = proc.inlets();
  inlets[m_pos].customData = m_old;
  proc.setInlets(std::move(inlets));
}

void SetInletData::redo() const
{
  Process::DataflowProcess& proc = m_model.find();
  auto inlets = proc.inlets();
  inlets[m_pos].customData = m_new;
  proc.setInlets(std::move(inlets));
}

void SetInletData::serializeImpl(DataStreamInput& s) const
{
    s << m_model << m_old << m_new << m_pos;
}

void SetInletData::deserializeImpl(DataStreamOutput& s)
{
    s >> m_model >> m_old >> m_new >> m_pos;
}


SetInletAddress::SetInletAddress(
        const Process::DataflowProcess& model,
    State::AddressAccessor n, int pos):
    m_model{model}
  , m_old{model.inlets()[pos].address}
  , m_new{n}
  , m_pos{pos}
{
}

void SetInletAddress::undo() const
{
  Process::DataflowProcess& proc = m_model.find();
  auto inlets = proc.inlets();
  inlets[m_pos].address = m_old;
  proc.setInlets(std::move(inlets));
}

void SetInletAddress::redo() const
{
  Process::DataflowProcess& proc = m_model.find();
  auto inlets = proc.inlets();
  inlets[m_pos].address = m_new;
  proc.setInlets(std::move(inlets));
}

void SetInletAddress::serializeImpl(DataStreamInput& s) const
{
    s << m_model << m_old << m_new << m_pos;
}

void SetInletAddress::deserializeImpl(DataStreamOutput& s)
{
    s >> m_model >> m_old >> m_new >> m_pos;
}
}
