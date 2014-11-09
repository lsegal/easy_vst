NAME = EasyVST
BUILDDIR = build
CPP = g++
ARCH = i386
INCLUDES = -Ivst3/ -I vst3/public.sdk/source/vst2.x/
LIBS = -framework Ruby
SOURCES = source/*.cpp vst3/public.sdk/source/vst2.x/*.cpp
VST = $(NAME)-$(ARCH).vst
BUNDLE = $(BUILDDIR)/$(VST)

all: $(BUNDLE)

clean: rm -rf $(BUILDDIR)

$(BUNDLE): $(BUNDLE)/Contents/Info.plist \
		$(BUNDLE)/Contents/Resources/PkgInfo \
		$(BUNDLE)/Contents/MacOS/$(NAME)
	cp -R $(BUNDLE) ~/Library/Audio/Plug-Ins/VST/$(VST)

$(BUNDLE)/Contents/Info.plist:
	mkdir -p $(BUNDLE)/Contents
	cp mac/Info.plist $(BUNDLE)/Contents/Info.plist

$(BUNDLE)/Contents/Resources/PkgInfo:
	mkdir -p $(BUNDLE)/Contents/Resources
	cp mac/PkgInfo $(BUNDLE)/Contents/Resources/PkgInfo

$(BUNDLE)/Contents/MacOS/$(NAME):
	mkdir -p $(BUNDLE)/Contents/MacOS
	$(CPP) -arch $(ARCH) $(INCLUDES) $(LIBS) -bundle \
	  -o $(BUNDLE)/Contents/MacOS/$(NAME) $(SOURCES)
