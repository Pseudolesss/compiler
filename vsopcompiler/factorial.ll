
%Main = type { i32 }
%IO = type { i32 }
%Object = type { i32 }

define i32 @Mainfactorial(%Main* %Object, i32 %n) {
methodfactorialentry:
  %n2 = alloca i32
  %Object1 = alloca %Main*
  store %Main* %Object, %Main** %Object1
  store i32 %n, i32* %n2
  %n3 = load i32, i32* %n2
  %lowtha = icmp slt i32 %n3, 2
  br i1 %lowtha, label %then, label %else

then:                                             ; preds = %methodfactorialentry
  br label %ifcont

else:                                             ; preds = %methodfactorialentry
  %n4 = load i32, i32* %n2
  %subop = fsub i32 %n4, 1
  %fctcall = call i32 @Mainfactorial(%Main* %0, i32 %subop)
  %n5 = load i32, i32* %n2
  %mulop = fmul i32 %n5, %fctcall
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %iftmp = phi i32 [ 1, %then ], [ %mulop, %else ]
  ret i32 %iftmp
}

define i32 @Mainmain(%Main* %Object) {
methodmainentry:
  %Object1 = alloca %Main*
  store %Main* %Object, %Main** %Object1
  %fctcall = call %IO @IOprint(%Main* %0, [48 x i8] c"\22Enter an integer greater-than or equal to 0: \22\00")

letbody:                                          ; No predecessors!
  %0 = alloca i32
  %fctcall2 = call i32 @IOinputInt32(%Main* %0)
  store i32 %fctcall2, i32* %0
  %n = load i32, i32* %0
  %lowtha = icmp slt i32 %n, 0
  br i1 %lowtha, label %then, label %else

then:                                             ; preds = %letbody
  %fctcall3 = call %IO @IOprint(%Main* %0, [56 x i8] c"\22Error: number must be greater-than or equal to 0.\5Cx0a\22\00")
  br label %ifcont

else:                                             ; preds = %letbody
  %fctcall4 = call %IO @IOprint(%Main* %0, [20 x i8] c"\22The factorial of \22\00")
  %member = getelementptr %IO, %IO %fctcall4, i32 0
  %1 = load %IO, %IO addrspace(9)* %member
  %n5 = load i32, i32* %0
  %fctcall6 = call %IO @IOprintInt32(<null operand!>, i32 %n5)
  %member7 = getelementptr %IO, %IO %fctcall6, i32 0
  %2 = load %IO, %IO addrspace(9)* %member7
  %fctcall8 = call %IO @IOprint(<null operand!>, [7 x i8] c"\22 is \22\00")
  %n9 = load i32, i32* %0
  %fctcall10 = call i32 @Mainfactorial(<null operand!>, i32 %n9)
  %fctcall11 = call %IO @IOprintInt32(<null operand!>, i32 %fctcall10)
  %member12 = getelementptr %IO, %IO %fctcall11, i32 0
  %3 = load %IO, %IO addrspace(9)* %member12
  %fctcall13 = call %IO @IOprint(<null operand!>, [7 x i8] c"\22\5Cx0a\22\00")
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %iftmp = phi i32 [ fsub (i32 0, i32 1), %then ], [ 0, %else ]
  ret i32 %iftmp
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