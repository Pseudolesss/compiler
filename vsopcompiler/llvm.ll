declare i32 @Mainfactorial(%Main*, i32)

declare i32 @Mainmain(%Main*)

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
  %malloccall = tail call i8* @malloc(i32 ptrtoint (%IO* getelementptr (%IO, %IO* null, i32 1) to i32))
  ret %IO* <badref>
}

declare noalias i8* @malloc(i32)

define %Main* @mallocMain() {
entry_mallocMain:
  %malloccall = tail call i8* @malloc(i32 ptrtoint (%Main* getelementptr (%Main, %Main* null, i32 1) to i32))
  ret %Main* <badref>
}

define %Object* @mallocObject() {
entry_mallocObject:
  %malloccall = tail call i8* @malloc(i32 ptrtoint (%Object* getelementptr (%Object, %Object* null, i32 1) to i32))
  ret %Object* <badref>
}

define i1* @mallocbool() {
entry_mallocbool:
  %malloccall = tail call i8* @malloc(i32 ptrtoint (i1* getelementptr (i1, i1* null, i32 1) to i32))
  ret i1* <badref>
}

define i32* @mallocint32() {
entry_mallocint32:
  %malloccall = tail call i8* @malloc(i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32))
  ret i32* <badref>
}

define i8** @mallocstring() {
entry_mallocstring:
  %malloccall = tail call i8* @malloc(i32 ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32))
  ret i8** <badref>
  %0 = call %Main* @mallocMain()
}