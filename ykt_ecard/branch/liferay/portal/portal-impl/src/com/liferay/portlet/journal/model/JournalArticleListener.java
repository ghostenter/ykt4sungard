/**
 * Copyright (c) 2000-2008 Liferay, Inc. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package com.liferay.portlet.journal.model;

import com.liferay.portal.ModelListenerException;
import com.liferay.portal.cms.servlet.CMSServletUtil;
import com.liferay.portal.model.BaseModel;
import com.liferay.portal.model.ModelListener;
import com.liferay.portal.servlet.filters.layoutcache.LayoutCacheUtil;
import com.liferay.portlet.journalcontent.util.JournalContentUtil;

/**
 * <a href="JournalArticleListener.java.html"><b><i>View Source</i></b></a>
 *
 * @author Brian Wing Shun Chan
 * @author Jon Steer
 *
 */
public class JournalArticleListener implements ModelListener {

	public void onBeforeCreate(BaseModel model) throws ModelListenerException {
	}

	public void onAfterCreate(BaseModel model) throws ModelListenerException {
	}

	public void onBeforeRemove(BaseModel model) throws ModelListenerException {
	}

	public void onAfterRemove(BaseModel model) throws ModelListenerException {
		clearCache(model);
	}

	public void onBeforeUpdate(BaseModel model) throws ModelListenerException {
	}

	public void onAfterUpdate(BaseModel model) throws ModelListenerException {
		clearCache(model);
	}

	protected void clearCache(BaseModel model) {

		// CMS

		CMSServletUtil.clearCache();

		// Journal content

		JournalContentUtil.clearCache();

		// Layout cache

		JournalArticleModel article = (JournalArticleModel)model;

		LayoutCacheUtil.clearCache(article.getCompanyId());
	}

}