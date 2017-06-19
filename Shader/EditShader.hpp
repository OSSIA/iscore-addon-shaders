#pragma once
#include <iscore/command/Command.hpp>
#include <Shader/ShaderModel.hpp>
#include <Shader/ShaderCommandFactory.hpp>

namespace Shader
{
class EditShader final : public iscore::Command
{
        ISCORE_COMMAND_DECL(Shader::CommandFactoryName(), EditShader, "Edit a shader")

    public:
        EditShader(const Shader::ProcessModel& model,
                   QString n);

        void undo(const iscore::DocumentContext& ctx) const override;
        void redo(const iscore::DocumentContext& ctx) const override;

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<ProcessModel> m_model;
        QString m_old;
        QString m_new;
};


class AddInlet final : public iscore::Command
{
    ISCORE_COMMAND_DECL(Shader::CommandFactoryName(), AddInlet, "Add an inlet")
    public:
      AddInlet(const Process::DataflowProcess& model);

      void undo(const iscore::DocumentContext& ctx) const override;
      void redo(const iscore::DocumentContext& ctx) const override;

  private:
    void serializeImpl(DataStreamInput & s) const override;
    void deserializeImpl(DataStreamOutput & s) override;

    Path<Process::DataflowProcess> m_model;
};

class SetInletData final : public iscore::Command
{
    ISCORE_COMMAND_DECL(Shader::CommandFactoryName(), SetInletData, "Set inlet data")
    public:
      SetInletData(const Process::DataflowProcess& model, QString dat, std::size_t pos);

      void undo(const iscore::DocumentContext& ctx) const override;
      void redo(const iscore::DocumentContext& ctx) const override;

  private:
    void serializeImpl(DataStreamInput & s) const override;
    void deserializeImpl(DataStreamOutput & s) override;

    Path<Process::DataflowProcess> m_model;
    QString m_old, m_new;
    quint64 m_pos;

};


class SetInletAddress final : public iscore::Command
{
    ISCORE_COMMAND_DECL(Shader::CommandFactoryName(), SetInletAddress, "Set inlet address")
    public:
      SetInletAddress(const Process::DataflowProcess& model, State::AddressAccessor dat, std::size_t pos);

      void undo(const iscore::DocumentContext& ctx) const override;
      void redo(const iscore::DocumentContext& ctx) const override;

  private:
    void serializeImpl(DataStreamInput & s) const override;
    void deserializeImpl(DataStreamOutput & s) override;

    Path<Process::DataflowProcess> m_model;
    State::AddressAccessor m_old, m_new;
    quint64 m_pos;

};
}
