/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2021 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#ifndef NODEPARAMVIEWITEM_H
#define NODEPARAMVIEWITEM_H

#include <QDockWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "node/node.h"
#include "nodeparamviewarraywidget.h"
#include "nodeparamviewconnectedlabel.h"
#include "nodeparamviewitembase.h"
#include "nodeparamviewkeyframecontrol.h"
#include "nodeparamviewwidgetbridge.h"
#include "widget/clickablelabel/clickablelabel.h"

namespace olive {

class NodeParamViewItemBody : public QWidget {
  Q_OBJECT
public:
  NodeParamViewItemBody(Node* node, QWidget* parent = nullptr);

  void SetTimeTarget(Node* target);

  void SetTime(const rational& time);

  void Retranslate();

  int GetElementY(NodeInput c) const;

  // Set the timebase of any timebased widgets contained here
   void SetTimebase(const rational& timebase);

signals:
  void RequestSetTime(const rational& time);

  void RequestSelectNode(const QVector<Node*>& node);

  void ArrayExpandedChanged(bool e);

private:
  void CreateWidgets(QGridLayout *layout, Node* node, const QString& input, int element, int row_index);

  void UpdateUIForEdgeConnection(const NodeInput &input);

  void PlaceWidgetsFromBridge(QGridLayout *layout, NodeParamViewWidgetBridge* bridge, int row);

  void InputArraySizeChangedInternal(Node *node, const QString &input, int size);

  struct InputUI {
    InputUI();

    QLabel* main_label;
    NodeParamViewWidgetBridge* widget_bridge;
    NodeParamViewConnectedLabel* connected_label;
    NodeParamViewKeyframeControl* key_control;
    QGridLayout* layout;
    int row;
    QPushButton *extra_btn;

    NodeParamViewArrayButton* array_insert_btn;
    NodeParamViewArrayButton* array_remove_btn;
  };

  QHash<NodeInput, InputUI> input_ui_map_;

  struct ArrayUI {
    QWidget* widget;
    int count;
    NodeParamViewArrayButton* append_btn;
  };

  Node *node_;

  QHash<NodeInputPair, ArrayUI> array_ui_;

  QHash<NodeInputPair, CollapseButton*> array_collapse_buttons_;

  rational timebase_;

  /**
   * @brief The column to place the keyframe controls in
   *
   * Serves as an effective "maximum column" index because the keyframe button is always aligned
   * to the right edge.
   */
  static const int kKeyControlColumn;

  static const int kArrayInsertColumn;
  static const int kArrayRemoveColumn;
  static const int kExtraButtonColumn;

  static const int kWidgetStartColumn;

private slots:
  void EdgeChanged(Node *output, const NodeInput &input);

  void ArrayCollapseBtnPressed(bool checked);

  void InputArraySizeChanged(const QString &input, int old_sz, int size);

  void ArrayAppendClicked();

  void ArrayInsertClicked();

  void ArrayRemoveClicked();

  void ToggleArrayExpanded();

  void ReplaceWidgets(const NodeInput& input);

  void ShowSpeedDurationDialogForNode();

};

class NodeParamViewItem : public NodeParamViewItemBase
{
  Q_OBJECT
public:
  NodeParamViewItem(Node* node, QWidget* parent = nullptr);

  void SetTimeTarget(Node* target);

  void SetTime(const rational& time);

  // Set the timebase of the NodeParamViewItemBody
  void SetTimebase(const rational& timebase);

  void SetHighlighted(bool e)
  {
    highlighted_ = e;

    update();
  }

  int GetElementY(const NodeInput& c) const;

signals:
  void RequestSetTime(const rational& time);

  void RequestSelectNode(const QVector<Node*>& node);

  void ArrayExpandedChanged(bool e);

protected:
  virtual void paintEvent(QPaintEvent *event) override;

protected slots:
  virtual void Retranslate() override;

private:
  NodeParamViewItemBody* body_;

  rational time_;

  bool highlighted_;

};

}

#endif // NODEPARAMVIEWITEM_H
