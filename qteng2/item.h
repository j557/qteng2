#pragma once

#include <QVector>
#include <QString>

class Item
{

public:
    Item( int id,
          const QString& question,
          const QString& answer,
          const QString& example,
          bool reversed,
		  unsigned int asked,
		  unsigned int answered );

    virtual ~Item() {}

	void SetAsked( unsigned int asked ) { m_asked = asked; m_rank = -100; }
    void IncreaseAsked() { m_asked++; m_rank = -100; }
	void SetAnswered( unsigned int answered ) { m_answered = answered; m_rank = -100; }
    void IncreaseAnswered() { m_answered++; m_rank = -100; }
	unsigned int GetAsked() const { return m_asked; }
	unsigned int GetAnswered() const { return m_answered; }
    const QString& GetQuestion() const { return m_question; }
    const QString& GetAnswer() const { return m_answer; }
    const QString& GetExample() const { return m_example; }
    int GetId() const { return m_id; }
    bool GetIsReversed() const { return m_reversed; }

	float GetRank() const;

	static bool CompareDebug(const Item* i1, const Item* i2);

private:
             int m_id;
	unsigned int m_asked;
	unsigned int m_answered;

    QString m_question;
    QString m_answer;
    QString m_example;

    bool m_reversed;

	mutable float m_rank;//to return the same value every time
};

bool Compare(const Item* i1, const Item* i2);
