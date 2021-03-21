/********************************************************
 *
 *             ______  ____    ______   ______
 *     /'\_/`\/\  _  \/\  _`\ /\__  _\ /\  _  \
 *    /\      \ \ \L\ \ \ \L\_\/_/\ \/ \ \ \L\ \
 *    \ \ \__\ \ \  __ \ \  _\/  \ \ \  \ \  __ \
 *     \ \ \_/\ \ \ \/\ \ \ \/    \_\ \__\ \ \/\ \
 *      \ \_\\ \_\ \_\ \_\ \_\    /\_____\\ \_\ \_\
 *       \/_/ \/_/\/_/\/_/\/_/    \/_____/ \/_/\/_/
 *
 *                  The Mafia project
 *
 ********************************************************
 *
 * File created by Anthony Ilareguy on 21/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_GAME_TYPES_VM_CONTEXT_H
#define DEF_MAFIA_CORE_GAME_TYPES_VM_CONTEXT_H

#include "serialization.h"
#include "game_value.h"
#include "game_variable.h"
#include "game_instruction.h"
#include "game_state.h"
#include "rv_allocator_local.h"
#include "debug_value.h"
#include "game_var_space.h"
#include "source_doc.h"
#include "../containers/auto_array.h"

namespace mafia::game_types
{
    class VMContext: public mafia::game_types::SerializeClass
    {
    public:
        class IDebugScope
        {  //ArmaDebugEngine
        public:
            virtual ~IDebugScope() = default;

            virtual const char* getName() const = 0;
            virtual int varCount() const = 0;
            virtual int getVariables(const _private::DebugVariable** storage, int count) const = 0;
            virtual DebugValue::RefType EvaluateExpression(const char* code, unsigned int rad) = 0;
            virtual void getSourceDocPosition(char* file, int fileSize, int& line) = 0;
            virtual IDebugScope* getParent() = 0;
        };

        //ArmaDebugEngine. Usual Intercept users won't need this and shouldn't use this
        class callstack_item: public mafia::RefCount, public IDebugScope
        {
        public:
            callstack_item* _parent;
            GameVarSpace _varSpace;

            int _stackEndAtStart;
            int _stackEnd;
            mafia::game_types::String _scopeName;

            virtual GameInstruction* next(int& d1, const GameState* s) { return nullptr; };

            virtual bool someEH(void* state) { return false; }

            virtual bool someEH2(void* state) { return false; };

            virtual mafia::game_types::String get_type() const = 0;

            virtual SerializationReturn serialize(ParamArchive& ar) { return SerializationReturn::no_error; }

            virtual void on_before_exec() {}
        };

        auto add_callstack_item(mafia::Ref<callstack_item> newItem);
        void throw_script_exception(GameValue value);
        bool is_scheduled() const;
        bool is_serialization_enabled() const;
        void disable_serialization();
        const SourceDocPosition& get_current_position() const;

        mafia::containers::AutoArray<mafia::Ref<callstack_item>,
                   mafia::game_types::RVAllocatorLocal<mafia::Ref<callstack_item>,
                                                       64>> callstack;  //#TODO check size on x64
        bool serialenabled; //disableSerialization -> true, 0x228
        void* dummyu; //VMContextBattlEyeMonitor : VMContextCallback

        //const bool is_ui_context; //no touchy
        mafia::containers::AutoArray<GameValue, mafia::game_types::RVAllocatorLocal<GameValue, 32>> scriptStack;

        SourceDoc sdoc;
        SourceDocPosition sdocpos;  //last instruction pos

        String name;  //profiler might like this

        //breakOut
        String breakscopename;
        //throw
        GameValue exception_value;  //0x4B0
        //breakOut
        GameValue breakvalue;
    private:
        uint32_t d[3];
        bool dumm;
        bool dumm2;             //undefined variables allowed?
        const bool scheduled;   //canSuspend 0x4D6
        bool local;
        bool doNil; //undefined variable will be set to nil (unscheduled). If this is false it will throw error
        //throw
        bool exception_state;   //0x4D9
        bool break_;            //0x4DA
        bool breakout;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_VM_CONTEXT_H
