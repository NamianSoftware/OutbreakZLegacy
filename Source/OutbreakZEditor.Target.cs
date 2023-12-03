// OutbreakZ

using UnrealBuildTool;
using System.Collections.Generic;

public class OutbreakZEditorTarget : TargetRules
{
	public OutbreakZEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "OutbreakZ" } );
	}
}
