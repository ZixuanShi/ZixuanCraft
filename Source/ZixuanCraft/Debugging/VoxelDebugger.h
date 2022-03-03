// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoxelDebugger.generated.h"

/** For debugging & testing purpose */
UCLASS()
class ZIXUANCRAFT_API AVoxelDebugger : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CubeCountXY = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CubeCountZ = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CubeLength = 100.0f;

protected:
	/** Returns Index of cube in AllCubes by X Y Z index */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetIndex(int32 X, int32 Y, int32 Z) const;

	/** Returns X Y Z index of cube in AllCubes by world location */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	FIntVector GetXYZFromLocation(FVector Location) const;

	/** Returns index of cube in AllCubes by world location */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetIndexFromLocation(FVector Location) const;
};
