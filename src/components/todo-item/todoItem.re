type todoId = int;

type todo = {
  id: todoId,
  value: string,
  completed: bool
};

let component = ReasonReact.statelessComponent("TodoItem");

[%bs.raw {| require('./todoItem.css') |}];

let make = (~todo, ~onCompletionChange, ~onDelete, _children) => {
  ...component,
  render: _self =>
    <div className="App-todo">
      <input
        _type="checkbox"
        className="App-todo-check"
        checked=(Js.Boolean.to_js_boolean(todo.completed))
        onChange=onCompletionChange
      />
      (ReasonReact.stringToElement(todo.value))
      <button className="App-todo-delete" onClick=onDelete>
        (ReasonReact.stringToElement("x"))
      </button>
    </div>
};