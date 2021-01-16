#include <Quasar.hpp>
#include <Quasar/Core/EntryPoint.hpp>

#include "EditorLayer.hpp"

namespace Quasar
{
    
    class QuasarEditor : public Application
    {
    public:
        QuasarEditor()
            : Application("Quasar Editor")
        {
            pushLayer(new EditorLayer());
        }

        ~QuasarEditor() 
        {
        }
    };

    Application *createApplication()
    {
        return new QuasarEditor();
    }
    
} // namespace Quasar

