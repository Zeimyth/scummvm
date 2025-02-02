/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef DIRECTOR_CAST_H
#define DIRECTOR_CAST_H

#include "common/hash-str.h"

namespace Common {
	class ReadStreamEndian;
	struct Rect;
	class SeekableReadStreamEndian;
}

namespace Director {

class Archive;
struct CastMemberInfo;
class CastMember;
class DirectorEngine;
class Lingo;
struct LingoArchive;
struct Resource;
class Stxt;
class BitmapCastMember;
class ScriptCastMember;
class ShapeCastMember;
class TextCastMember;

typedef Common::HashMap<byte, byte> CharMap;
typedef Common::HashMap<uint16, uint16> FontSizeMap;
struct FontXPlatformInfo {
	Common::String toFont;
	bool remapChars;
	FontSizeMap sizeMap;
};
typedef Common::HashMap<Common::String, FontXPlatformInfo *> FontXPlatformMap;

class Cast {
public:
	Cast(Movie *movie, uint16 castLibID, bool shared = false);
	~Cast();

	void loadArchive();
	void setArchive(Archive *archive);
	Archive *getArchive() const { return _castArchive; };
	Common::String getMacName() const { return _macName; }

	bool loadConfig();
	void loadCast();
	void loadCastDataVWCR(Common::SeekableReadStreamEndian &stream);
	void loadCastData(Common::SeekableReadStreamEndian &stream, uint16 id, Resource *res);
	void loadCastInfo(Common::SeekableReadStreamEndian &stream, uint16 id);
	void loadLingoContext(Common::SeekableReadStreamEndian &stream);

	void loadCastChildren();
	void loadSoundCasts();

	void copyCastStxts();
	Common::Rect getCastMemberInitialRect(int castId);
	void setCastMemberModified(int castId);
	CastMember *getCastMember(int castId);
	CastMember *getCastMemberByName(const Common::String &name);
	CastMember *getCastMemberByScriptId(int scriptId);
	CastMemberInfo *getCastMemberInfo(int castId);
	const Stxt *getStxt(int castId);
	Common::String getVideoPath(int castId);

	void dumpScript(const char *script, ScriptType type, uint16 id);

	uint16 mapFont(uint16 id);

private:
	PaletteV4 loadPalette(Common::SeekableReadStreamEndian &stream);
	void loadScriptText(Common::SeekableReadStreamEndian &stream, uint16 id);
	void loadFontMap(Common::SeekableReadStreamEndian &stream);
	void loadFXmp(Common::SeekableReadStreamEndian &stream);
	bool readFXmpLine(Common::SeekableReadStreamEndian &stream);
	Common::String getString(Common::String str);

public:
	Archive *_castArchive;
	uint16 _version;
	uint16 _castLibID;

	Common::HashMap<uint16, uint16> _fontMap;
	CharMap _macCharsToWin;
	CharMap _winCharsToMac;
	FontXPlatformMap _macFontsToWin;
	FontXPlatformMap _winFontsToMac;

	Common::HashMap<int, CastMember *> *_loadedCast;
	Common::HashMap<int, const Stxt *> *_loadedStxts;
	uint16 _castIDoffset;
	uint16 _castArrayStart;
	uint16 _castArrayEnd;

	Common::Rect _movieRect;
	uint16 _stageColor;
	int _defaultPalette;

	LingoArchive *_lingoArchive;

private:
	DirectorEngine *_vm;
	Lingo *_lingo;
	Movie *_movie;

	bool _isShared;

	Common::String _macName;

	Common::HashMap<uint16, CastMemberInfo *> _castsInfo;
	Common::HashMap<Common::String, int, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> _castsNames;
	Common::HashMap<uint16, int> _castsScriptIds;
};

} // End of namespace Director

#endif
