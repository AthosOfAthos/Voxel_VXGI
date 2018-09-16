// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Voxel_VXGIEditorTarget : TargetRules
{
	public Voxel_VXGIEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Voxel_VXGI" } );
	}
}
