#pragma once

enum class InstructionType {
	R_TYPE,
	I_TYPE,
	S_TYPE,
	SB_TYPE,
	U_TYPE,
	UJ_TYPE,
	INVALID_TYPE
};

enum class TokenType {
	INSTRUCTION,
	REGISTER,
	LABEL,
	DIRECTIVE,
	IMMEDIATE,
	INVALID
};

enum class DirectiveType {
	TEXT,
	DATA,
	BYTE,
	HALF,
	WORD,
	DWORD,
	ASCIIZ,
	UNKNOWN
};