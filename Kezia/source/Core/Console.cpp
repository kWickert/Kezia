#include <gl/glew.h>

#include "Console.h"

#include "Core/Keys.h"
#include "Core/Time.h"
#include "Core/Windows_ResourceManager.h"

#include "Graphics/GraphicsCommon.h"

#include "Math/MathUtility.h"

#include "Event/Event.h"
#include "Event/EventManager.h"

#include <functional>
#include <sstream>

namespace Kezia
{
	Console::Console()
		:	m_IsActive(false),
			m_CursorPosition(0),
			m_Color(Color::White),
			m_Font(g_FileResourceManager->GetWorkingDirectory() + "\\data\\fonts\\arial.ttf"),
			m_SelectedMatchIndex(0),
			m_SelectedHistoryIndex(0)
	{
		g_EventManager->RegisterObjectForEvent("keyPress", *this, &Console::HandleKeyPress);
		g_EventManager->RegisterObjectForEvent("keyRelease", *this, &Console::HandleKeyRelease);
		g_EventManager->RegisterObjectForEvent("console.DeletePrevious", *this, &Console::HandleContinuousInput);
		g_EventManager->RegisterObjectForEvent("console.DeleteNext", *this, &Console::HandleContinuousInput);
		g_EventManager->RegisterObjectForEvent("console.CursorLeft", *this, &Console::HandleContinuousInput);
		g_EventManager->RegisterObjectForEvent("console.CursorRight", *this, &Console::HandleContinuousInput);

		RegisterCommand("quit", Quit);
	}

	Console::~Console()
	{}

	void Console::HandleKeyPress(NamedProperties & arguments)
	{
		U8 key;
		arguments.GetProperty("key", key);

		//std::cout << "key " << key << "(" << (int)key << ")" << std::endl;		

		switch(key)
		{
		case '`':
		case '~':
			ToggleConsole();
			break;
		case 13: //enter
			ExecuteCommandLine();
			break;
		case 8: //backspace
			ScheduleDelayedEventFire(0.15, "console.DeletePrevious", true, arguments);
			DeletePreviousCharacter();
			break;
		case 27:
			exit(1);
			break;
		case '\t':
			Autocomplete();
			break;
		case KZ_DELETE: //delete
			ScheduleDelayedEventFire(0.15, "console.DeleteNext", true, arguments);
			DeleteNextCharacter();
			break;
		case KZ_LEFT:
			ScheduleDelayedEventFire(0.15, "console.CursorLeft", true, arguments);
			RetreatCursor();
			break;
		case KZ_RIGHT:
			ScheduleDelayedEventFire(0.15, "console.CursorRight", true, arguments);
			AdvanceCursor();
			break;
		case KZ_UP:
			HandleCycleInput(1);
			break;
		case KZ_DOWN:
			HandleCycleInput(-1);
			break;
		case KZ_HOME:
			Home();
			break;
		case KZ_END:
			End();
			break;
		default:
			if(key < SpecialKeyBegin)
			{
				InsertCharacter(static_cast<char>(key));
			}
			break;
		}
	}

	void Console::HandleKeyRelease(NamedProperties & arguments)
	{
		U8 key;
		arguments.GetProperty("key", key);

		//std::cout << "key " << key << "(" << (int)key << ")" << std::endl;		

		switch(key)
		{
		case 8: //backspace
			CancelEventFire("console.DeletePrevious");
			break;
		case KZ_DELETE:
			CancelEventFire("console.DeleteNext");
		case KZ_LEFT:
			CancelEventFire("console.CursorLeft");
			break;
		case KZ_RIGHT:
			CancelEventFire("console.CursorRight");
			break;
		}
	}

	void Console::ToggleConsole()
	{
		m_IsActive = !m_IsActive;
	}

	void Console::Autocomplete()
	{
		if(m_SelectedMatchIndex < static_cast<S32>(m_PossibleCommandMatches.size()))
		{
			SetCommandLineText(m_PossibleCommandMatches[m_SelectedMatchIndex]);

			m_PossibleCommandMatches.empty();
			m_DrawAutocomplete = false;
		}
	}

	void Console::AdvanceCursor()
	{
		if(!m_IsActive)
			return;

		m_CursorPosition++;

		m_CursorPosition = std::min<U32>(m_CursorPosition, m_CommandLine.length());
	}

	void Console::RetreatCursor()
	{
		if(!m_IsActive)
			return;

		if(m_CursorPosition != 0)
			m_CursorPosition--;
	}

	void Console::Home()
	{
		if(!m_IsActive)
			return;

		m_CursorPosition = 0;
	}

	void Console::End()
	{
		if(!m_IsActive)
			return;

		m_CursorPosition = m_CommandLine.size();
	}

	void Console::InsertCharacter(char c)
	{		
		if(!m_IsActive)
			return;

		m_CommandLine.insert(m_CursorPosition, 1, c);
		AdvanceCursor();	

		PopulateAutocomplete();
	}

	void Console::DeleteNextCharacter()
	{
		if(!m_IsActive)
			return;

		m_CommandLine.erase(m_CursorPosition, 1);

		PopulateAutocomplete();
	}

	void Console::DeletePreviousCharacter()
	{
		if(!m_IsActive)
			return;

		if(m_CursorPosition != 0)
		{
			m_CommandLine.erase(m_CursorPosition - 1, 1);
			RetreatCursor();
		}

		PopulateAutocomplete();
	}

	void Console::SetCommandLineText(const std::string & text)
	{
		m_CommandLine = text;
		m_CursorPosition = text.size();

		PopulateAutocomplete();
	}

	void Console::Print(const std::string & text)
	{
		m_ConsoleLog.Push(text);
	}

	void Console::ExecuteCommandLine()
	{
		std::stringstream ss(m_CommandLine);

		std::string command;
		ss >> command;

		std::string args(ss.str().substr(command.size()));

		if(ExecuteCommand(command.c_str(), std::stringstream(args)))
		{
			m_CommandHistory.push_back(m_CommandLine);
			m_SelectedHistoryIndex = 0;

			SetCommandLineText("");
		}
	}

	void Console::Render()
	{
		if(!m_IsActive)
			return;

		g_Renderer->DisableDepthTesting();

		static Texture blankTexture("C:\\Users\\ghdemo\\Documents\\School\\Projects\\Engine\\Kezia\\data\\textures\\blank.png");

		DrawTexturedQuad(blankTexture, Color(0, 0, 0, 0.75f));

		F32 extent = 1.0f;
		//Matrix4<F32>::Orthographic(-extent, extent, -extent, extent);

		Vector2<F32> positions[4] =
		{
			Vector2<F32>(-extent, -extent),
			Vector2<F32>(extent, -extent),
			Vector2<F32>(extent, extent),
			Vector2<F32>(extent, -extent)
		};
		
		static const F32 logBegin = 0.2f;
		F32 cursorY = logBegin;
		auto it = m_ConsoleLog.Begin();

		m_Font.SetFontColor(Color::White);

		do 
		{
			if(m_ConsoleLog.Size() == 0)
				break;

			--it;

			m_Font.RenderText(*it, Vector3<F32>(-extent, -extent + cursorY, 0));
			cursorY += 0.1f;
		} while (it != m_ConsoleLog.Begin());

		std::string output = (":") + m_CommandLine;

		U32 seconds = static_cast<U32>(GetLiveTimeSeconds());
		if(seconds % 2)
		{
			F32 cursorX = m_Font.Measure(output.substr(0, m_CursorPosition + 1).c_str()) - m_Font.Measure("|") / 2.0f;

			m_Font.RenderText("|", Vector3<F32>(-extent + cursorX, -extent + 0.1f, 0));
		}

		m_Font.RenderText(output, Kezia::Vector3<F32>(-extent, -extent + 0.1f, 0));

		if(m_DrawAutocomplete)
		{
			cursorY = logBegin;

			F32 maxWidth(0);
			F32 totalHeight(0);

			for(auto it = m_PossibleCommandMatches.begin(); it != m_PossibleCommandMatches.end(); ++it)
			{
				F32 width = m_Font.Measure(*it);
				if(width > maxWidth)
				{
					maxWidth = width;
				}

				totalHeight += 0.1f;
			}

			//TODO fix this
			DrawTexturedQuad(nullptr, blankTexture, nullptr, Vector3<F32>(-extent + maxWidth * 0.5f, -extent + logBegin * 0.875f + totalHeight * 0.5f, 0), Vector2<F32>(maxWidth / 2.0f, totalHeight / 2.0f), Matrix3<F32>(), Color(0, 0, 0, 1.0f));

			S32 i = 0;
			for(auto it = m_PossibleCommandMatches.begin(); it != m_PossibleCommandMatches.end(); ++it)
			{
				m_Font.RenderText(*it, i == m_SelectedMatchIndex ? Color(1) : Color(0.5f), Vector3<F32>(-extent, -extent + cursorY, 0));
				cursorY += 0.1f;

				++i;
			}
		}

		g_Renderer->EnableDepthTesting();
	}

	void Console::HandleContinuousInput(NamedProperties & arguments)
	{
		U8 key;
		arguments.GetProperty("key", key);

		//std::cout << "key " << key << "(" << (int)key << ")" << std::endl;		

		switch(key)
		{
		case 8: //backspace
			DeletePreviousCharacter();
			break;
		case KZ_DELETE:
			DeleteNextCharacter();
			break;
		case KZ_LEFT:
			RetreatCursor();
			break;
		case KZ_RIGHT:
			AdvanceCursor();
			break;
		}
	}

	void Console::PopulateAutocomplete()
	{
		m_PossibleCommandMatches.clear();

		if(m_CommandLine.empty())
		{
			m_DrawAutocomplete = false;
		}
		else
		{
			for(auto it = m_Commands.Begin(); it != m_Commands.End(); ++it)
			{
				const std::string & commandName = (*it).first.key;
				if(commandName.find(m_CommandLine) != std::string::npos)
				{
					m_PossibleCommandMatches.push_back(commandName);
				}
			}

			m_DrawAutocomplete = true;
			m_SelectedMatchIndex = 0;
		}
	}

	void Console::HandleCycleInput(S8 direction)
	{
		if(m_DrawAutocomplete)
		{
			m_SelectedMatchIndex += direction;

			if(m_SelectedMatchIndex < 0)
			{
				m_SelectedMatchIndex = m_PossibleCommandMatches.size() - 1;
			}
			else if(m_SelectedMatchIndex >= static_cast<S32>(m_PossibleCommandMatches.size()))
			{
				m_SelectedMatchIndex = 0;
			}
		}
		else
		{
			m_SelectedHistoryIndex -= direction;

			if(m_SelectedHistoryIndex < 0)
			{
				m_SelectedHistoryIndex = m_CommandHistory.size() - 1;
			}
			else if(m_SelectedHistoryIndex >= static_cast<S32>(m_CommandHistory.size()))
			{
				m_SelectedHistoryIndex = 0;
			}

			SetCommandLineText(m_CommandHistory[m_SelectedHistoryIndex]);
			m_DrawAutocomplete = false;
		}
	}
}