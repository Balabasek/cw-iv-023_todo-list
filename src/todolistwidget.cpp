#include "todolistwidget.h"
#include "taskinputwindow.h"
#include "Node.h"
#include "listitemwidget.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>

enum TaskRoles
{
    TaskText = Qt::UserRole + 1,
    TaskDate = Qt::UserRole + 2,
    TaskPriority = Qt::UserRole + 3
};

ToDoListWidget::ToDoListWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    tasksListWidget = new QListWidget(this);

    vBox->addWidget(tasksListWidget);
}

void ToDoListWidget::createTask()
{
    inputDialog = new TaskInputDialog(this);
    connect(inputDialog, SIGNAL(taskInfoSended(const Node&)), this, SLOT(addedTask(const Node&)));
    inputDialog->open();
    qDebug() << "Create task";
}

void ToDoListWidget::editTask()
{
    if (!tasksListWidget->currentItem())
    {
        return;
    }

    inputDialog = new TaskInputDialog(this);
    connect(inputDialog, SIGNAL(taskInfoSended(const Node&)), this, SLOT(editingTask(const Node&)));

    QString taskText = tasksListWidget->currentItem()->data(TaskText).toString();
    QString taskDate = tasksListWidget->currentItem()->data(TaskDate).toString();
    int taskPriority = tasksListWidget->currentItem()->data(TaskPriority).toInt();
    Node editableTask(taskDate.toStdString(), taskText.toStdString(), taskPriority);
    inputDialog->setTaskData(editableTask);
    qDebug() << "Edit task";
    inputDialog->open();
}

void ToDoListWidget::editingTask(const Node& node)
{
    QString taskText = QString::fromStdString(node.GetText());
    QString dateText = QString::fromStdString(node.GetDate());
    int priority = node.GetPriority();
    auto taskItem = tasksListWidget->currentItem();
    taskItem->setData(TaskText, taskText);
    taskItem->setData(TaskDate, dateText);
    taskItem->setData(TaskPriority, priority);
    tasksListWidget->removeItemWidget(tasksListWidget->currentItem());
    tasksListWidget->setItemWidget(taskItem, new ListItemWidget(taskText, dateText, tasksListWidget));
}

void ToDoListWidget::compliteTask()
{
    qDebug() << "complite task";
}

void ToDoListWidget::deleteTask()
{
    tasksListWidget->removeItemWidget(tasksListWidget->currentItem());
    delete tasksListWidget->takeItem(tasksListWidget->currentRow());
    qDebug() << "Delete task";
}

void ToDoListWidget::setTaskPriority(int priority)
{
    qDebug() << "Set priority:" << priority;
}

void ToDoListWidget::addedTask(const Node& newTask)
{
    inputDialog = new TaskInputDialog(this);
    connect(inputDialog, SIGNAL(taskInfoSended(const Node&)), this, SLOT(addedTask(const Node&)));

    QListWidgetItem* taskItem = new QListWidgetItem(tasksListWidget);
    tasksListWidget->addItem(taskItem);
    QString taskText = QString::fromStdString(newTask.GetText());
    QString dateText = QString::fromStdString(newTask.GetDate());
    taskItem->setData(TaskText, taskText);
    taskItem->setData(TaskDate, dateText);
    taskItem->setData(TaskPriority, newTask.GetPriority());
    ListItemWidget* listItemWidget = new ListItemWidget(taskText, dateText, tasksListWidget);
    taskItem->setSizeHint(listItemWidget->sizeHint());
    tasksListWidget->setItemWidget(taskItem, listItemWidget);

}