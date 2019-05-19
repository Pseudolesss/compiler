%Main = type { i32 }
%IO = type { i32 }
%Object = type { i32 }

define i32 @Mainmain(%Main* %Object) {
methodmainentry:
  %Object1 = alloca %Main*
  store %Main* %Object, %Main** %Object1
  %fctcall = call %IO @IOprint(%Main* %0, [48 x i8] c"\22Enter an integer greater-than or equal to 0: \22\00")
  ret i32 0
}

declare %IO @IOprintBool(%IO*, i1)

declare %IO @IOprintInt32(%IO*, i32)

declare i8* @IOinputLine(%IO*)

declare %IO @IOprint(%IO*, i8*)

declare i32 @IOinputInt32(%IO*)

declare i1 @IOinputBool(%IO*)

declare %IO @IOprintBool.1(%IO*, i1)

declare %IO @IOprintInt32.2(%IO*, i32)

declare i8* @IOinputLine.3(%IO*)

declare %IO @IOprint.4(%IO*, i8*)

declare i32 @IOinputInt32.5(%IO*)

declare i1 @IOinputBool.6(%IO*)

define %IO* @mallocIO() {
entry_mallocIO:
  %malloccall = tail call i8* @malloc(i8 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i8))
  ret %IO* %malloccall
}

declare noalias i8* @malloc(i8)

define %Main* @mallocMain() {
entry_mallocMain:
  %malloccall = tail call i8* @malloc(i8 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i8))
  ret %Main* %malloccall
}

define %Object* @mallocObject() {
entry_mallocObject:
  %malloccall = tail call i8* @malloc(i8 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i8))
  ret %Object* %malloccall
}

define i1* @mallocbool() {
entry_mallocbool:
  %malloccall = tail call i8* @malloc(i8 ptrtoint (i1* getelementptr (i1, i1* null, i32 1) to i8))
  ret i1* %malloccall
}

define i32* @mallocint32() {
entry_mallocint32:
  %malloccall = tail call i8* @malloc(i8 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i8))
  ret i32* %malloccall
}

define i8** @mallocstring() {
entry_mallocstring:
  %malloccall = tail call i8* @malloc(i8 ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i8))
  ret i8** %malloccall
}

define i32 @main() {
main:
  %0 = call %Main* @mallocMain()
  %1 = call i32 @Mainmain(%Main* %0)
  ret i32 %1
}