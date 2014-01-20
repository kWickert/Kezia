#pragma once

#include "Core/Any.h"
#include "Core/CircularBuffer.h"
#include "Core/Clock.h"
#include "Core/HashedRegistry.h"
#include "Core/Singleton.h"
#include "Core/TypeList.h"

#include "Graphics/Color.h"
#include "Graphics/FreetypeFont.h"

#include <algorithm>
#include <string>
#include <vector>

namespace Kezia
{
	class NamedProperties;

	class Console
	{
	public:
		Console();
		~Console();

		void Update(const F64 deltaSeconds);

		void HandleKeyPress(NamedProperties & arguments);
		void HandleKeyRelease(NamedProperties & arguments);

		void ToggleConsole();

		void Autocomplete();

		void AdvanceCursor();
		void RetreatCursor();

		void Home();
		void End();

		void InsertCharacter(char c);

		void DeleteNextCharacter();
		void DeletePreviousCharacter();

		void SetCommandLineText(const std::string & text);

		virtual void Print(const std::string & text);

		void Render();

		void ExecuteCommandLine();

		template<typename T>
		bool ExecuteCommand(const std::string & name, T args)
		{
			auto find = m_Commands.Find(name);

			bool commandFound = find != m_Commands.End();
			if(!commandFound)
			{
				std::stringstream ss;

				ss << "Command, " << name << ", not found";

				Print(ss.str());
			}
			else
			{
				find->second->Run(args);
			}

			return commandFound;
		}

		template <typename R>
		void RegisterCommand(const std::string & name, R (*callback)())
		{
			m_Commands.Insert(name, new Command<TYPELIST_1(R)>(name, callback));
		}

		template <typename R, typename T>
		void RegisterCommand(const std::string & name, R (*callback)(T))
		{
			m_Commands.Insert(name, new Command<TYPELIST_2(R, T)>(name, callback));
		}

		template <typename R, typename T1, typename T2>
		void RegisterCommand(const std::string & name, R (*callback)(T1, T2))
		{
			m_Commands.Insert(name, new Command<TYPELIST_3(R, T1, T2)>(name, callback));
		}
	private:
		class CommandBase
		{
		public:
			CommandBase()
			{}

			virtual ~CommandBase()
			{}

			virtual Any Run(const std::vector<Any> & args) = 0;
			virtual Any Run(std::stringstream & stream) = 0;
		};

		template <class TList> struct Command;

		//typelist wraps command types (arguments and return)
		template<class Head, class Tail>
		struct Command<TypeList<Head, Tail> > : public CommandBase
		{
			typedef TypeList<Head, Tail> TList;

			typedef typename ToFunction<TList>::Result FunctionSignature;
			FunctionSignature Callback;

			Command(const std::string & name, FunctionSignature fn)
				:	Callback(fn)
			{}

			~Command()
			{}

			bool Check(const std::vector<Any> & args)
			{
				return (args.size() == Length<TList>::value - 1) && TypeCheck<Tail>::Check(args);
			}

			virtual Any Run(const std::vector<Any> & args)
			{
				if(Check(args))
					return Executor<Head, FunctionSignature, Length<TList>::value - 1>::Run(Callback, args);
				else
				{
					std::cerr << "Bad args\n";

					return Any();
				}
			}

			virtual Any Run(std::stringstream & stream)
			{
				return Run(AnyExtractor<Tail, Length<Tail>::value>::Parse(stream));
			}
		};

		void HandleCycleInput(S8 direction);
		void HandleContinuousInput(NamedProperties & arguments);

		void PopulateAutocomplete();

		bool m_IsActive;

		FreetypeFont m_Font;

		//log stuff
		//std::vector<std::pair<std::string, Color> > m_ConsoleLog;
		CircularBuffer<std::string, 50> m_ConsoleLog;

		std::string m_CommandLine;

		bool m_DrawAutocomplete;
		S32 m_SelectedMatchIndex;
		std::vector<std::string> m_PossibleCommandMatches;

		S32 m_SelectedHistoryIndex;
		std::vector<std::string> m_CommandHistory;

		Color m_Color;

		U32 m_CursorPosition;

		//console commands
		//std::map<U32, CommandBase *> m_Commands;

		HashedRegistry<CommandBase *> m_Commands;
	};

	inline void Quit()
	{
		exit(1);
	}

	extern Console * g_Console;
}