#ifndef ITEMSCONTAINER_H
#define ITEMSCONTAINER_H

#include <QList>
#include <QString>

class Item;

class ItemContainerInterface
{
public:
    /* method which should be called by user to prepare next question
     * As result of this method called, getCurrentQuestion() method's
     * return value should change if necessary. If there is no next
     * question in containrer, should set current question to NULL */
    virtual void prepareNextQuestion()                = 0;

    /* returns current question item. May return NULL. */
    virtual const Item* getCurrentQuestion() const    = 0;

    /* should be called to indicate, that answer for current question
     * was correct */
    virtual void currentQuestionAnsweredProperly()    = 0;

    /* should be called to indicate, that answer for current question
     * was incorrect */
    virtual void currentQuestionAnsweredNotProperly() = 0;

    /* return number of questions that was asked. In fact,
     * this is correlated to number of prepareNextQuestion calls */
    virtual int getNumberOfAskedQuestions() const     = 0;

    /* return number of correct answers. */
    virtual int getNumberOfCorrectAnswers() const     = 0;

    /* return number of questions that are left to be asked.
     * Negative value indicates, that number of questions left cannot
     * be specified. Note, that depended on container algorithm,
     * number of questions left may not be equal to total - asked */
    virtual int getNumberOfQuestionsLeft() const      = 0;

    /* number of items loaded to container */
    virtual int getNumberOfQuestionsTotal() const     = 0;

    /* load items from file */
    virtual bool load( const QString& filename )      = 0;

    /* clear and reload data */
    virtual bool reload()                             = 0;
};

class ItemContainer : public ItemContainerInterface
{
    typedef QList<Item*> ItemList;

public:
    ItemContainer();

public: //ItemsContainerInterface
    virtual void prepareNextQuestion();
    virtual const Item* getCurrentQuestion() const;
    virtual void currentQuestionAnsweredProperly();
    virtual void currentQuestionAnsweredNotProperly();
    virtual int  getNumberOfAskedQuestions() const;
    virtual int  getNumberOfCorrectAnswers() const;
    virtual int  getNumberOfQuestionsLeft() const;
    virtual int  getNumberOfQuestionsTotal() const;
    virtual bool load( const QString& filename );
    virtual bool reload();

private:
    void clear();

private:
            QString            m_filename;
    mutable int                m_asked;
            int                m_answered;
    mutable bool               m_nextPrepared;
            Item*              m_currentItem;
            ItemList           m_items;
            ItemList           m_answeredItems;
            int                m_questionsCount;
};

#endif // ITEMSCONTAINER_H
