#pragma once
#include <score/command/Command.hpp>

#include <Shader/ShaderCommandFactory.hpp>
#include <Shader/ShaderModel.hpp>

namespace Shader
{
class EditShader final : public score::Command
{
  SCORE_COMMAND_DECL(Shader::CommandFactoryName(), EditShader, "Edit a shader")

public:
  EditShader(const Shader::ProcessModel& model, QString n);

  void undo(const score::DocumentContext& ctx) const override;
  void redo(const score::DocumentContext& ctx) const override;

protected:
  void serializeImpl(DataStreamInput& s) const override;
  void deserializeImpl(DataStreamOutput& s) override;

private:
  Path<ProcessModel> m_model;
  QString m_old;
  QString m_new;
};

/*
class AddInlet final : public score::Command
{
    S CORE_COMMAND_DECL(Shader::CommandFactoryName(), AddInlet, "Add an inlet")
    public:
      AddInlet(const Process::ProcessModel& model);

      void undo(const score::DocumentContext& ctx) const override;
      void redo(const score::DocumentContext& ctx) const override;

  private:
    void serializeImpl(DataStreamInput & s) const override;
    void deserializeImpl(DataStreamOutput & s) override;

    Path<Process::ProcessModel> m_model;
};

class SetInletData final : public score::Command
{
    SC ORE_COMMAND_DECL(Shader::CommandFactoryName(), SetInletData, "Set inlet
data") public: SetInletData(const Process::ProcessModel& model, QString dat,
std::size_t pos);

      void undo(const score::DocumentContext& ctx) const override;
      void redo(const score::DocumentContext& ctx) const override;

  private:
    void serializeImpl(DataStreamInput & s) const override;
    void deserializeImpl(DataStreamOutput & s) override;

    Path<Process::ProcessModel> m_model;
    QString m_old, m_new;
    quint64 m_pos;

};


class SetInletAddress final : public score::Command
{
    S CORE_COMMAND_DECL(Shader::CommandFactoryName(), SetInletAddress, "Set
inlet address") public: SetInletAddress(const Process::ProcessModel& model,
State::AddressAccessor dat, std::size_t pos);

      void undo(const score::DocumentContext& ctx) const override;
      void redo(const score::DocumentContext& ctx) const override;

  private:
    void serializeImpl(DataStreamInput & s) const override;
    void deserializeImpl(DataStreamOutput & s) override;

    Path<Process::ProcessModel> m_model;
    State::AddressAccessor m_old, m_new;
    quint64 m_pos;
};
*/
}
