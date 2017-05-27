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
}
