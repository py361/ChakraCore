//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "Backend.h"

CompileAssert(sizeof(FixedFieldIDL) == sizeof(FixedFieldInfo));

/* static */
void
FixedFieldInfo::PopulateFixedField(_In_opt_ Js::Type * type, _In_opt_ Js::Var var, _Out_ FixedFieldInfo * fixed)
{
    FixedFieldIDL * rawFF = fixed->GetRaw();
    rawFF->fieldValue = (intptr_t)var;
    rawFF->nextHasSameFixedField = false;
    if (var != nullptr && Js::JavascriptFunction::Is(var))
    {
        Js::JavascriptFunction * funcObj = Js::JavascriptFunction::FromVar(var);
        rawFF->valueType = ValueType::FromObject(funcObj).GetRawData();
        rawFF->funcInfoAddr = (intptr_t)funcObj->GetFunctionInfo();
        rawFF->isClassCtor = funcObj->GetFunctionInfo()->IsClassConstructor();
        rawFF->localFuncId = (intptr_t)funcObj->GetFunctionInfo()->GetLocalFunctionId();
        if (Js::ScriptFunction::Is(var))
        {
            rawFF->environmentAddr = (intptr_t)Js::ScriptFunction::FromVar(funcObj)->GetEnvironment();
        }
    }
    if (type != nullptr)
    {
        JITType::BuildFromJsType(type, (JITType*)&rawFF->type);
    }
}

void
FixedFieldInfo::SetNextHasSameFixedField()
{
    m_data.nextHasSameFixedField = TRUE;
}

bool
FixedFieldInfo::IsClassCtor() const
{
    return m_data.isClassCtor != FALSE;
}

bool
FixedFieldInfo::NextHasSameFixedField() const
{
    return m_data.nextHasSameFixedField != FALSE;
}

uint
FixedFieldInfo::GetLocalFuncId() const
{
    return m_data.localFuncId;
}

ValueType
FixedFieldInfo::GetValueType() const
{
    CompileAssert(sizeof(ValueType) == sizeof(uint16));
    return *(ValueType*)&m_data.valueType;
}

intptr_t
FixedFieldInfo::GetFieldValue() const
{
    return m_data.fieldValue;
}

intptr_t
FixedFieldInfo::GetFuncInfoAddr() const
{
    return m_data.funcInfoAddr;
}

intptr_t
FixedFieldInfo::GetEnvironmentAddr() const
{
    return m_data.environmentAddr;
}

JITType *
FixedFieldInfo::GetType() const
{
    return (JITType*)&m_data.type;
}

FixedFieldIDL *
FixedFieldInfo::GetRaw()
{
    return &m_data;
}

