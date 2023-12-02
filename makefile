#コンパイラ
CC		= gcc

#コンパイルオプション
CFLAGS	= -lm

#実行ファイル名
TARGET	= out

#コンパイルファイル
SRCDIR	= _src
SRCS	= $(wildcard $(SRCDIR)/*.c)

#中間実行ファイル生成
OBJDIR	=
OBJS	= $(addprefix $(OBJDIR), $(notdir $(SRCS:.c=.o)))

#インクルードディレクトリ
INCDIR	= -I _include

#ライブラリディレクトリパス
LIBDIR	=

#追加するライブラリファイル
LIBS	= 

#ターゲットファイルの生成
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS)

#オブジェクトファイル生成
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(SRCS)

#"make all" で make clean & make を同時に行う
all: clean $(OBJS) $(TARGET)

#.oファイル、実行ファイル、.dファイル削除
clean:
	-rm -f $(OBJS) $(TARGET) *.d