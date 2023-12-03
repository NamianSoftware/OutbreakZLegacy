// OutbreakZ

using UnrealBuildTool;
using System.Collections.Generic;

public class OutbreakZTarget : TargetRules
{
	public OutbreakZTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "OutbreakZ" } );
	}
}
