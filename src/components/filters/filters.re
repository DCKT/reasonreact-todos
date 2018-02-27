type t =
  | All
  | Completed
  | Remaining;

let component = ReasonReact.statelessComponent("Filters");

[%bs.raw {| require('./filters.css') |}];

let setClassName = (~default="", ~optionals=?, ()) =>
  default
  ++ " "
  ++ String.concat(
       " ",
       switch optionals {
       | None => []
       | Some(list) => list
       }
     );

let make = (~filter, ~onFilterClick, _children) => {
  ...component,
  render: _self =>
    <div className="App-filters">
      <button
        className=(
          setClassName(
            ~default="App-filter",
            ~optionals=[filter == All ? "App-filter--active" : ""],
            ()
          )
        )
        onClick=(_event => onFilterClick(All))>
        (ReasonReact.stringToElement("All"))
      </button>
      <button
        className=(
          setClassName(
            ~default="App-filter",
            ~optionals=[filter == Completed ? "App-filter--active" : ""],
            ()
          )
        )
        onClick=(_event => onFilterClick(Completed))>
        (ReasonReact.stringToElement("Completed"))
      </button>
      <button
        className=(
          setClassName(
            ~default="App-filter",
            ~optionals=[filter == Remaining ? "App-filter--active" : ""],
            ()
          )
        )
        onClick=(_event => onFilterClick(Remaining))>
        (ReasonReact.stringToElement("Remaining"))
      </button>
    </div>
};