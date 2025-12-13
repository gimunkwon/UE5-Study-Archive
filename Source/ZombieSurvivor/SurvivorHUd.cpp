#include "Misc/AutomationTest.h"

IMPLEMENT_COMPLEX_AUTOMATION_TEST(SurvivorHUd, "DefaultNamespace.SurvivorHUd", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void SurvivorHUd::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const
{
    OutBeautifiedNames.Add(TEXT("SurvivorHUd_TestName"));
    OutTestCommands.Add(TEXT("SurvivorHUd_TestName"));
    
}

bool SurvivorHUd::RunTest(const FString& Parameters)
{
    return true;
}