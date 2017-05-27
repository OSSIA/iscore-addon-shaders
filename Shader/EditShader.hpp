#pragma once
#include <iscore/command/Command.hpp>
#include <Shader/ShaderModel.hpp>
#include <Shader/ShaderCommandFactory.hpp>

namespace Shader
{
class EditShader : public iscore::Command
{
        ISCORE_COMMAND_DECL(Shader::CommandFactoryName(), EditShader, "Edit a shader")

    public:
        EditShader(const Shader::ProcessModel& model,
                   QString n);

        void undo() const override;
        void redo() const override;

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<ProcessModel> m_model;
        QString m_old;
        QString m_new;
};
}
