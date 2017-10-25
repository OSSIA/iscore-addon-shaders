#include "EditShader.hpp"
#include <score/model/path/PathSerialization.hpp>

namespace Shader
{
EditShader::EditShader(
        const Shader::ProcessModel& model
    , QString n):
    m_model{model}
  , m_old{model.shader()}
  , m_new{n}
{
}

void EditShader::undo(const score::DocumentContext& ctx) const
{
    Shader::ProcessModel& proc = m_model.find(ctx);
    proc.setShader(m_old);
}

void EditShader::redo(const score::DocumentContext& ctx) const
{
    Shader::ProcessModel& proc = m_model.find(ctx);
    proc.setShader(m_new);
}

void EditShader::serializeImpl(DataStreamInput& s) const
{
    s << m_model << m_old << m_new;
}

void EditShader::deserializeImpl(DataStreamOutput& s)
{
    s >> m_model >> m_old >> m_new;
}


/*
AddInlet::AddInlet(
        const Process::ProcessModel& model):
    m_model{model}
{
}

void AddInlet::undo(const score::DocumentContext& ctx) const
{
  Process::ProcessModel& proc = m_model.find(ctx);
  auto inlets = proc.inlets();
  inlets.resize(inlets.size() - 1);
  proc.setInlets(std::move(inlets));
}

void AddInlet::redo(const score::DocumentContext& ctx) const
{
  Process::ProcessModel& proc = m_model.find(ctx);
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
        const Process::ProcessModel& model,
    QString n, std::size_t pos):
    m_model{model}
  , m_old{model.inlets()[pos].customData}
  , m_new{n}
  , m_pos{pos}
{
}

void SetInletData::undo(const score::DocumentContext& ctx) const
{
  Process::ProcessModel& proc = m_model.find(ctx);
  auto inlets = proc.inlets();
  inlets[m_pos].customData = m_old;
  proc.setInlets(std::move(inlets));
}

void SetInletData::redo(const score::DocumentContext& ctx) const
{
  Process::ProcessModel& proc = m_model.find(ctx);
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
        const Process::ProcessModel& model,
    State::AddressAccessor n, std::size_t pos):
    m_model{model}
  , m_old{model.inlets()[pos].address}
  , m_new{n}
  , m_pos{pos}
{
}

void SetInletAddress::undo(const score::DocumentContext& ctx) const
{
  Process::ProcessModel& proc = m_model.find(ctx);
  auto inlets = proc.inlets();
  inlets[m_pos].address = m_old;
  proc.setInlets(std::move(inlets));
}

void SetInletAddress::redo(const score::DocumentContext& ctx) const
{
  Process::ProcessModel& proc = m_model.find(ctx);
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
*/
}
