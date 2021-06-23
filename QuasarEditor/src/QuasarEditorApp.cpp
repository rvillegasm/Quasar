#include <Quasar.hpp>
#include <Quasar/Core/EntryPoint.hpp>

#include "EditorLayer.hpp"

namespace Quasar
{
    
    class QuasarEditor : public Application
    {
    public:
        QuasarEditor(ApplicationCommandLineArgs args)
            : Application("Quasar Editor", args)
        {
            pushLayer(new EditorLayer());
        }

        ~QuasarEditor() 
        {
        }
    };

    Application *createApplication(ApplicationCommandLineArgs args)
    {
        return new QuasarEditor(args);
    }
    
} // namespace Quasar

