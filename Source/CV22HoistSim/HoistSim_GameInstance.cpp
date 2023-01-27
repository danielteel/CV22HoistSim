// Fill out your copyright notice in the Description page of Project Settings.


#include "HoistSim_GameInstance.h"


void UHoistSim_GameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Startup"))
	nodeProcHandle = FPlatformProcess::CreateProc(TEXT("C:\\Program Files\\nodejs\\node.exe"), TEXT("C:\\hoistsim_server\\app.js"), true, false, false, nullptr, 0, nullptr, nullptr);
}

void UHoistSim_GameInstance::Shutdown() {
	UE_LOG(LogTemp, Warning, TEXT("Shutdown"))
	FPlatformProcess::TerminateProc(nodeProcHandle);
}