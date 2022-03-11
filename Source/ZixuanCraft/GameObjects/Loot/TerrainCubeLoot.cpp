// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainCubeLoot.h"
#include "UObject/ConstructorHelpers.h"

ATerrainCubeLoot::ATerrainCubeLoot()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube"));
	MeshComp->SetStaticMesh(Cube.Object);
	MeshComp->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
}

