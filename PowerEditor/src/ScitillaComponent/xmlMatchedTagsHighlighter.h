//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef XMLMATCHEDTAGSHIGHLIGHTER_H
#define XMLMATCHEDTAGSHIGHLIGHTER_H

class ScintillaEditView;

enum TagCateg {tagOpen, tagClose, inSingleTag, outOfTag, invalidTag, unknownPb};

class XmlMatchedTagsHighlighter {
public:
	XmlMatchedTagsHighlighter(ScintillaEditView *pEditView):_pEditView(pEditView){};
	void tagMatch(bool doHiliteAttr);

private:
	struct XmlMatchedTagsPos {
		DOCPOSITION tagOpenStart;
		DOCPOSITION tagNameEnd;
		DOCPOSITION tagOpenEnd;

		DOCPOSITION tagCloseStart;
		DOCPOSITION tagCloseEnd;
	};

	ScintillaEditView *_pEditView;

	DOCPOSITION getFirstTokenPosFrom(DOCPOSITION targetStart, DOCPOSITION targetEnd, const char *token, std::pair<DOCPOSITION, DOCPOSITION> & foundPos);
	TagCateg getTagCategory(XmlMatchedTagsPos & tagsPos, DOCPOSITION curPos);
	bool getMatchedTagPos(DOCPOSITION searchStart, DOCPOSITION searchEnd, const char *tag2find, const char *oppositeTag2find, std::vector<DOCPOSITION> oppositeTagFound, XmlMatchedTagsPos & tagsPos);
	bool getXmlMatchedTagsPos(XmlMatchedTagsPos & tagsPos);
	std::vector< std::pair<DOCPOSITION, DOCPOSITION> > getAttributesPos(DOCPOSITION start, DOCPOSITION end);
	bool isInList(DOCPOSITION element, std::vector<DOCPOSITION> elementList) {
		for (size_t i = 0 ; i < elementList.size() ; i++)
			if (element == elementList[i])
				return true;
		return false;
	};
};

#endif //XMLMATCHEDTAGSHIGHLIGHTER_H

