#pragma once
#include <score/application/ApplicationContext.hpp>
#include <score/plugins/InterfaceList.hpp>
#include <score/plugins/qt_interfaces/CommandFactory_QtInterface.hpp>
#include <score/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp>
#include <score/plugins/qt_interfaces/PluginRequirements_QtInterface.hpp>

#include <QObject>

#include <utility>
#include <vector>

class score_addon_shader final : public score::Plugin_QtInterface,
                                 public score::FactoryInterface_QtInterface,
                                 public score::CommandFactory_QtInterface
{
  SCORE_PLUGIN_METADATA(1, "0c549f8f-db48-4823-acbd-11cb9d0bc354")

public:
  score_addon_shader();
  virtual ~score_addon_shader();

private:
  // Defined in FactoryInterface_QtInterface
  std::vector<std::unique_ptr<score::InterfaceBase>> factories(
      const score::ApplicationContext& ctx,
      const score::InterfaceKey& key) const override;

  // Defined in CommandFactory_QtInterface
  std::pair<const CommandGroupKey, CommandGeneratorMap>
  make_commands() override;
};
